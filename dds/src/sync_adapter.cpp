
#include "sync_adapter.h"

CSyncAdapter::CSyncAdapter(std::string topic) :
        m_pDataPublisher(nullptr),
        m_pDataSubscriber(nullptr)
{
    m_ddsTopic = topic;
}

CSyncAdapter::~CSyncAdapter()
{

}

bool CSyncAdapter::Initialize(DataNotifyCallback newDataCallback,
                              DataNotifyCallback disposedDataCallback)
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

    if (m_pDataPublisher->Initialize() == false ||
        m_pDataSubscriber->Initialize() == false)
    {
        delete m_pDataPublisher;
        m_pDataPublisher = nullptr;
        delete m_pDataSubscriber;
        m_pDataSubscriber = nullptr;
        return false;
    }

    CCortoDataSubscriber::DataDelegate newDelegate(shared_from_this(),
        [this, newDataCallback](CCortoDataSubscriber::Sample &sample)
        {
            if ((this->m_uuid == sample.data().source()) == false)
            {
                newDataCallback(sample);
            }
        }
    );
    m_pDataSubscriber->RegisterNewDataSubscriber(newDelegate);

    CCortoDataSubscriber::DataDelegate disposedDelegate(shared_from_this(),
        [this, disposedDataCallback](CCortoDataSubscriber::Sample &sample)
        {
            if ((this->m_uuid == sample.data().source()) == false)
            {
                disposedDataCallback(sample);
            }
        }
    );
    m_pDataSubscriber->RegisterInstanceNotAliveSubscriber(disposedDelegate);

    return true;
}

bool CSyncAdapter::SendData(std::string type, std::string parent, std::string name, std::string value)
{
    if (m_pDataPublisher == nullptr)
    {
        return false;
    }

    std::string key = parent+"/"+name;
    Corto::Data data(type, parent, name, value, m_uuid);
    dds::core::InstanceHandle handle = m_dataHandlers[key];
    if (handle.is_nil())
    {
        handle = m_pDataPublisher->RegisterInstance(data);
        m_dataHandlers[key] = handle;
    }
    m_pDataPublisher->Write(data, handle);
    return true;
}

bool CSyncAdapter::UnregisterData(std::string parent, std::string name)
{
    bool retVal = false;

    std::string key = parent+"/"+name;
    dds::core::InstanceHandle handle = m_dataHandlers[key];
    if (handle.is_nil() == false)
    {
        m_pDataPublisher->UnregisterInstance(handle);
    }

    return retVal;
}

void CSyncAdapter::Close()
{
    if (m_pDataSubscriber != nullptr)
    {
        m_pDataSubscriber->UnregisterNewDataSubscriber(shared_from_this());
        delete m_pDataSubscriber;
        m_pDataSubscriber = nullptr;
    }

    if (m_pDataPublisher != nullptr)
    {
        HandlerMap::iterator handlerIt;
        for (handlerIt = m_dataHandlers.begin();
             handlerIt != m_dataHandlers.end();
             handlerIt++)
        {
            m_pDataPublisher->UnregisterInstance(handlerIt->second);
        }

        delete m_pDataPublisher;
        m_pDataPublisher = nullptr;
    }
}

bool CSyncAdapter::Query(SampleSeq &sampleSeq, std::string expression, ParamVector params)
{
    bool retVal = false;

    ReadRetCode retCode = m_pDataSubscriber->Query(sampleSeq, expression, params);

    if (retCode == READ_RET_OK || retCode == READ_RET_NO_DATA )
    {
        retVal = true;
    }

    return retVal;
}

bool CSyncAdapter::Query(SampleSeq &sampleSeq, std::string expression)
{
    bool retVal = false;

    ReadRetCode retCode = m_pDataSubscriber->Query(sampleSeq, expression);

    if (retCode == READ_RET_OK || retCode == READ_RET_NO_DATA )
    {
        retVal = true;
    }

    return retVal;
}
