
#include "sync_adapter.h"

#define MAX_WAIT_TIME 5
#define KEY(parent, id) parent+"/"+id
#define INSTANCE_KEY(src, dest, id) src+":"+dest+":"+id

#define TO_LOWER(c) if (c <= 'Z' && c >= 'A') c += 32

// #define TRACE(fmt, args...) printf("%s:%i, %s: " fmt "\n", __FILE__, __LINE__, __func__, args)
#define TRACE(fmt, args...)

std::size_t StringHasher::operator ()(const std::string &k) const
{
    const std::size_t FNV_prime = 1099511628211;
    const std::size_t offset_basic = 14695981039346656037U;

    uint8_t c = 0;

    std::size_t hash = offset_basic;

    const char *it = k.c_str();
    while ((c = *it++) != '\0')
    {
        hash = (hash ^ c) * FNV_prime;
    }

    return hash;
}

bool StringEquals::operator ()(const std::string &lhs, const std::string &rhs) const
{
    bool retVal = lhs.size() == rhs.size();

    char c1, c2;
    const char *s1 = lhs.c_str();
    const char *s2 = lhs.c_str();

    while (retVal && (c1 = *s1++) != '\0' && (c2 = *s2++) != '\0')
    {
        retVal = c1 == c2;
    }

    return retVal;
}

enum MessageType
{
    CONNECTION = 1,
    SUBSCRIPTION = 2,
    UPDATE_REQUEST = 3,
    UPDATE_DONE = 4
};

static void StrToLower(std::string &str)
{
    size_t size = str.size();
    for (size_t i = 0; i < size; i++)
    {
        char &c = str[i];
        TO_LOWER(c);
    }
}


void CSyncAdapter::ProcessEvent()
{
    UniqueLock lock(m_ebMtx);
    EventMap events = std::move(m_eventBuffer);
    lock.unlock();

    for (EventMap::iterator iter = events.begin();
         iter != events.end();
         iter++)
    {
        Event &event = iter->second;
        if (event.m_event == Event::Type::CREATE)
        {
            CreateData(event);
        }
        else if (event.m_event == Event::Type::UPDATE)
        {
            UpdateData(event);
        }
        else if (event.m_event == Event::Type::DELETE)
        {
            DeleteData(event);
        }
    }
}

bool CSyncAdapter::CreateData(Event &event)
{
    if (m_pDataPublisher == nullptr)
    {
        return false;
    }

    Corto::Data &data = event.m_data;
    std::string key = KEY(data.parent(), data.id());

    LockGuard lock(m_ddsMtx);
    ObjectData &obj = m_objects[key];

    obj.m_data = std::move(data);
    if (obj.m_handle.is_nil())
    {
        obj.m_owner = true;
        obj.m_handle = m_pDataPublisher->RegisterInstance(obj.m_data);
    }

    obj.m_data.owner(obj.m_owner);
    TRACE("CreateData parent = %s AND id = %s", obj.m_data.parent().c_str(), obj.m_data.id().c_str());
    m_pDataPublisher->Write(obj.m_data, obj.m_handle);

    return true;
}

bool CSyncAdapter::UpdateData(Event &event)
{
    if (m_pDataPublisher == nullptr)
    {
        return false;
    }

    Corto::Data &data = event.m_data;
    std::string key = KEY(data.parent(), data.id());

    LockGuard lock(m_ddsMtx);
    ObjectData &obj = m_objects[key];

    obj.m_data = std::move(data);

    if (obj.m_handle.is_nil())
    {
        obj.m_owner = false;
        obj.m_handle = m_pDataPublisher->RegisterInstance(obj.m_data);
    }

    if (obj.m_owner == false || obj.m_subscribers.size() > 0)
    {
        obj.m_data.owner(obj.m_owner);
        TRACE("[%s]: DDS Write %s Update", m_uuid.c_str(), key.c_str());
        m_pDataPublisher->Write(obj.m_data, obj.m_handle);
    }
    else
    {
        TRACE("[%s]: Cache %s Update", m_uuid.c_str(), key.c_str());
        m_updateCache[key] = obj.m_data.value();
    }

    return true;
}

bool CSyncAdapter::DeleteData(Event &event)
{
    DisposeData(event.m_data);

    return true;
}

void CSyncAdapter::DisposeData(const Corto::Data &data)
{
    if (m_pDataPublisher != nullptr)
    {
        std::string key = KEY(data.parent(), data.id());

        LockGuard lock(m_ddsMtx);
        ObjectData &obj = m_objects[key];

        if (obj.m_handle.is_nil() == false)
        {
            TRACE("[%s] %s",m_uuid.c_str(), key.c_str());
            m_pDataPublisher->UnregisterInstance(obj.m_handle);
            obj.m_handle = DdsInstanceHandle::nil();
        }
        m_updateCache.erase(key);
    }
}

void CSyncAdapter::SendUpdates()
{
    LockGuard lock(m_ddsMtx);
    CacheMap updates = std::move(m_updateCache);

    for (CacheMap::iterator iter = updates.begin();
         iter != updates.end();
         iter++)
    {
        const std::string &key = iter->first;
        const std::string &value = iter->second;

        ObjectData &obj = m_objects[key];
        obj.m_data.value(value);
        obj.m_data.owner(obj.m_owner);

        if (obj.m_handle.is_nil() == false)
        {
            TRACE("[%s]: DDS Write %s Update", m_uuid.c_str(), key.c_str());
            m_pDataPublisher->Write(obj.m_data, obj.m_handle);
        }
    }
}


CSyncAdapter::DdsInstanceHandle CSyncAdapter::SendMessage(const std::string &dest, const std::string &id, uint8_t type)
{
    if (m_pMessagePublisher == nullptr)
    {
        return DdsInstanceHandle::nil();
    }

    //DEBUG
    // std::string ts = (type == MessageType::CONNECTION) ? "CONNECTION" :
    //                  (type == MessageType::SUBSCRIPTION) ? "SUBSCRIPTION" :
    //                  (type == MessageType::UPDATE_REQUEST) ? "UPDATE_REQUEST" :
    //                  (type == MessageType::UPDATE_DONE) ? "UPDATE_DONE" :
    //                  "UNKNOWN";
    // TRACE("[%s]: DDS Sends Message %s", m_uuid.c_str(), ts.c_str());

    std::string key = INSTANCE_KEY(m_uuid, dest, id);

    Corto::Message msg;
    msg.source(m_uuid);
    msg.destination(std::move(dest));
    msg.id(std::move(id));
    msg.type(type);

    LockGuard lock(m_instancesMtx);
    DdsInstanceHandle &handle = m_instances[key];

    if (handle.is_nil() == true)
    {
        handle = m_pMessagePublisher->RegisterInstance(msg);
    }

    m_pMessagePublisher->Write(msg, handle);

    return handle;
}

void CSyncAdapter::DisposeMessage(const std::string &dest, const std::string &id)
{
    if (m_pMessagePublisher != nullptr)
    {
        std::string key = INSTANCE_KEY(m_uuid, dest, id);
        LockGuard lock(m_instancesMtx);
        InstanceMap::iterator iter = m_instances.find(key);
        if (iter != m_instances.end())
        {
            DdsInstanceHandle &handle = iter->second;
            if (handle.is_nil() == false)
            {
                m_pMessagePublisher->UnregisterInstance(handle);
            }
            m_instances.erase(iter);
        }
    }
}

void CSyncAdapter::OnCortoMessageDispose(CCortoMessageSubscriber::Sample &sample)
{
    const Corto::Message &msg = sample.data();
    if (msg.destination().empty() == true || msg.destination() == m_uuid)
    {
        MessageType type = (MessageType)msg.type();
        switch (type) {
            case MessageType::CONNECTION:
            {
                TRACE("CONNECTION: %s, CLOSE", msg.source().c_str());
                LockGuard lock(m_ddsMtx);
                m_ddsConnectors.erase(msg.source());
                break;
            }
            case MessageType::SUBSCRIPTION:
            {
                TRACE("SUBSCRIPTION: %s, %s, CLOSE", msg.source().c_str(), msg.id().c_str());
                LockGuard lock(m_ddsMtx);
                ObjectData &obj = m_objects[msg.id()];
                obj.m_subscribers.erase(msg.source());
                break;
            }
            case MessageType::UPDATE_REQUEST:
            {
                TRACE("UPDATE_REQUEST: %s, CLOSE", msg.source().c_str());
                DisposeMessage(msg.source(), msg.id());
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void CSyncAdapter::OnCortoMessage(CCortoMessageSubscriber::Sample &sample)
{
    const Corto::Message &msg = sample.data();

    if (msg.destination().empty() == true || msg.destination() == m_uuid)
    {
        MessageType type = (MessageType)msg.type();
        switch (type) {
            case MessageType::CONNECTION:
            {
                TRACE("CONNECTION: %s", msg.source().c_str());
                LockGuard lock(m_ddsMtx);
                m_ddsConnectors.insert(msg.source());
                break;
            }
            case MessageType::SUBSCRIPTION:
            {
                TRACE("SUBSCRIPTION: %s, %s", msg.source().c_str(), msg.id().c_str());
                LockGuard lock(m_ddsMtx);
                ObjectData &obj = m_objects[msg.id()];
                obj.m_subscribers.insert(msg.source());
                break;
            }
            case MessageType::UPDATE_REQUEST:
            {
                TRACE("UPDATE_REQUEST: %s", msg.source().c_str());
                SendUpdates();
                SendMessage(msg.source(), msg.id(), MessageType::UPDATE_DONE);
                break;
            }
            case MessageType::UPDATE_DONE:
            {
                TRACE("UPDATE_DONE: %s", msg.source().c_str());
                LockGuard lock(m_requestMtx);
                m_requests[msg.id()]--;
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

CSyncAdapter::CSyncAdapter(std::string topic) :
        m_pDataPublisher(nullptr),
        m_pDataSubscriber(nullptr),
        m_pMessagePublisher(nullptr),
        m_pMessageSubscriber(nullptr),
        m_taskId(0),
        m_lastID(0)
{
    m_ddsTopic = topic;
}

CSyncAdapter::~CSyncAdapter()
{

}

bool CSyncAdapter::Initialize(DataNotifyCallback newDataCallback,
                              DataNotifyCallback disposedDataCallback,
                              uint64_t updateRate)
{
    CRemuvUuid uuid;
    uuid.Generate();
    m_uuid = uuid.GetStr();

    if (m_pDataPublisher == nullptr)
    {
        m_pDataPublisher = new CCortoDataPublisher(m_ddsTopic);
    }

    if (m_pDataSubscriber == nullptr)
    {
        m_pDataSubscriber = new CCortoDataSubscriber(m_ddsTopic);
    }

    if (m_pMessagePublisher == nullptr)
    {
        m_pMessagePublisher = new CCortoMessagePublisher("Msg_"+m_ddsTopic);
    }

    if (m_pMessageSubscriber == nullptr)
    {
        m_pMessageSubscriber = new CCortoMessageSubscriber("Msg_"+m_ddsTopic);
    }

    if (m_pDataPublisher->Initialize() == false ||
        m_pDataSubscriber->Initialize() == false ||
        m_pMessagePublisher->Initialize() == false ||
        m_pMessageSubscriber->Initialize() == false)
    {
        delete m_pDataPublisher;
        m_pDataPublisher = nullptr;
        delete m_pDataSubscriber;
        m_pDataSubscriber = nullptr;
        delete m_pMessageSubscriber;
        m_pMessageSubscriber = nullptr;
        delete m_pMessagePublisher;
        m_pMessageSubscriber = nullptr;
        return false;
    }

    CCortoDataSubscriber::DataDelegate newDelegate(shared_from_this(),
        [this, newDataCallback](CCortoDataSubscriber::Sample &sample)
        {
            if ((this->m_uuid == sample.data().source()) == false)
            {
                TRACE("NEW_DATA %s/%s", sample.data().parent().c_str(), sample.data().id().c_str());
                newDataCallback(sample);
            }
        }
    );
    m_pDataSubscriber->RegisterNewDataSubscriber(newDelegate);

    CCortoDataSubscriber::DataDelegate disposedDelegate(shared_from_this(),
        [this, disposedDataCallback](CCortoDataSubscriber::Sample &sample)
        {
            if ((this->m_uuid != sample.data().source()))
            {
                if (sample.data().owner() == true)
                {
                    DisposeData(sample.data());
                    disposedDataCallback(sample);
                }
            }
        }
    );
    m_pDataSubscriber->RegisterInstanceNotAliveSubscriber(disposedDelegate);

    CCortoMessageSubscriber::DataDelegate msgDelegate(shared_from_this(),
        [this](CCortoMessageSubscriber::Sample &sample)
        {
            if ((this->m_uuid != sample.data().source()))
            {
                OnCortoMessage(sample);
            }
        }
    );
    m_pMessageSubscriber->RegisterNewDataSubscriber(msgDelegate);

    CCortoMessageSubscriber::DataDelegate msgDisposedDelegate(shared_from_this(),
        [this](CCortoMessageSubscriber::Sample &sample)
        {
            if ((this->m_uuid != sample.data().source()))
            {
                OnCortoMessageDispose(sample);
            }
        }
    );
    m_pMessageSubscriber->RegisterInstanceNotAliveSubscriber(msgDisposedDelegate);

    if (m_taskId == 0)
    {
        m_taskId = m_taskFactory.Create(updateRate, updateRate, std::bind(&CSyncAdapter::ProcessEvent, this));
    }

    TRACE("[%s]:%s", m_uuid.c_str(), m_ddsTopic.c_str());

    SendMessage("", "0", MessageType::CONNECTION);

    return true;
}

bool CSyncAdapter::CreateData(std::string &type, std::string &parent, std::string &id, std::string &name, std::string &value)
{
    StrToLower(parent);
    StrToLower(id);

    std::string key = KEY(parent, id);

    LockGuard lock(m_ebMtx);

    Event &event = m_eventBuffer[key];

    event.m_event = Event::Type::CREATE;
    event.m_data.type(std::move(type));
    event.m_data.parent(std::move(parent));
    event.m_data.id(std::move(id));
    event.m_data.name(std::move(name));
    event.m_data.value(std::move(value));
    event.m_data.source(m_uuid);

    return true;
}

bool CSyncAdapter::UpdateData(std::string &type, std::string &parent, std::string &id, std::string &name, std::string &value)
{
    StrToLower(parent);
    StrToLower(id);

    std::string key = KEY(parent, id);

    LockGuard lock(m_ebMtx);

    Event &event = m_eventBuffer[key];
    if (event.m_event != Event::Type::CREATE)
    {
        event.m_event = Event::Type::UPDATE;
    }
    event.m_data.type(std::move(type));
    event.m_data.parent(std::move(parent));
    event.m_data.id(std::move(id));
    event.m_data.name(std::move(name));
    event.m_data.value(std::move(value));
    event.m_data.source(m_uuid);

    return true;
}

bool CSyncAdapter::DeleteData(std::string &parent, std::string &id)
{
    StrToLower(parent);
    StrToLower(id);

    std::string key = KEY(parent, id);

    LockGuard lock(m_ebMtx);

    Event &event = m_eventBuffer[key];
    event.m_event = Event::Type::DELETE;
    event.m_data.parent(std::move(parent));
    event.m_data.id(std::move(id));

    return true;
}

bool CSyncAdapter::SubscribeData(std::string &parent, std::string &expr)
{
    StrToLower(parent);
    StrToLower(expr);

    std::string objPath = KEY(parent, expr);

    DdsInstanceHandle handle = SendMessage("", objPath, MessageType::SUBSCRIPTION);

    return true;
}

bool CSyncAdapter::UnsubscribeData(std::string &parent, std::string &expr)
{
    StrToLower(parent);
    StrToLower(expr);

    std::string objPath = KEY(parent, expr);

    DisposeMessage("", objPath);

    return true;
}

bool CSyncAdapter::Query(SampleSeq &sampleSeq,
                         std::string &parent,
                         std::string &expr,
                         bool latest)
{
    StrToLower(parent);
    StrToLower(expr);
    // std::string objPath = KEY(parent, expr);

    bool retVal = false;

    if (latest == true)
    {
        bool wait = true;
        bool timeOut = false;

        UniqueLock requestLock(m_requestMtx);
        std::string requestId = std::to_string(++m_lastID);

        UniqueLock ddsLock(m_ddsMtx);
        m_requests[requestId] = m_ddsConnectors.size();
        ddsLock.unlock();

        requestLock.unlock();
        for (std::string s : m_ddsConnectors)
        {
            TRACE("Connections: %s", s.c_str());
        }

        SendUpdates();

        TRACE("%s: %lu", requestId.c_str(), m_ddsConnectors.size());

        SendMessage("", requestId, MessageType::UPDATE_REQUEST);

        time_t start = time(0);
        while (wait == true && timeOut == false)
        {
            std::this_thread::yield();

            requestLock.lock();
            wait = m_requests[requestId] > 0;
            requestLock.unlock();

            time_t end = time(0);
            timeOut = (end - start) > MAX_WAIT_TIME;
        }

        DisposeMessage("", requestId);

        requestLock.lock();
        m_requests.erase(requestId);
    }
    TRACE("QUERY parent = %s AND id = %s", parent.c_str(), expr.c_str());
    ParamVector params = {parent, expr};
    std::string expression = "parent = %0 AND id like %1";
    ReadRetCode retCode = m_pDataSubscriber->Query(sampleSeq,expression,params);

    if (retCode == READ_RET_OK || retCode == READ_RET_NO_DATA )
    {
        retVal = true;
    }

    return retVal;
}

void CSyncAdapter::Close()
{
    DisposeMessage("", "0");

    // LockGuard lock(m_ebMtx);
    // LockGuard requestLock(m_requestMtx);
    // LockGuard instanceLock(m_instancesMtx);
    // LockGuard ddsLock(m_ddsMtx);

    if (m_taskId != 0)
    {
        m_taskFactory.Destroy(m_taskId);
        m_taskId = 0;
    }

    if (m_pDataSubscriber != nullptr)
    {
        m_pDataSubscriber->UnregisterNewDataSubscriber(shared_from_this());
        m_pDataSubscriber->UnregisterInstanceNotAliveSubscriber(shared_from_this());

        delete m_pDataSubscriber;
        m_pDataSubscriber = nullptr;
    }

    if (m_pDataPublisher != nullptr)
    {
        ObjectMap::iterator objIt;
        for (objIt = m_objects.begin();
             objIt != m_objects.end();
             objIt++)
        {
            m_pDataPublisher->UnregisterInstance(objIt->second.m_handle);
        }
        m_objects.clear();

        delete m_pDataPublisher;
        m_pDataPublisher = nullptr;
    }

    if (m_pMessageSubscriber != nullptr)
    {
        m_pMessageSubscriber->UnregisterNewDataSubscriber(shared_from_this());
        m_pMessageSubscriber->UnregisterInstanceNotAliveSubscriber(shared_from_this());

        delete m_pMessageSubscriber;
        m_pMessageSubscriber = nullptr;
    }

    if (m_pMessagePublisher != nullptr)
    {
        InstanceMap::iterator instanceIt;
        for (instanceIt = m_instances.begin();
             instanceIt != m_instances.end();
             instanceIt++)
        {
            m_pMessagePublisher->UnregisterInstance(instanceIt->second);
        }
        m_instances.clear();

        delete m_pMessagePublisher;
        m_pMessagePublisher = nullptr;
    }
}
