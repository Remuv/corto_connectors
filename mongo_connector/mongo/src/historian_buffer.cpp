#include <recorto/mongo_connector/mongo/historian_buffer.h>
#include <recorto/common/common.h>

#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/builder/core.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/pipeline.hpp>

#define DEFAULT_WAIT_US 10
#define TO_MILLISECONDS(time_point) std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch())

#define CORTO_NULL_OWNER(func) do { \
    corto_object prev = corto_setOwner(nullptr); \
    func; \
    corto_setOwner(prev); \
} while(false)

typedef std::chrono::hours Hours;
typedef std::chrono::minutes Minutes;
typedef std::chrono::seconds Seconds;
typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::microseconds Microseconds;

using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using bsoncxx::types::b_date;
using bsoncxx::types::b_array;
using bsoncxx::types::b_document;
using bsoncxx::document::element;


static b_date ISODate(Minutes minutes);
static b_date ISODate(Milliseconds millis);

static mongocxx::cursor MongoHistorySelect(
    MongoClientPtr &pClient,
    std::string &database,
    std::string &collection,
    std::string &id,
    optional<std::chrono::milliseconds> start,
    optional<std::chrono::milliseconds> end,
    optional<uint64_t> skip,
    optional<uint64_t> limit,
    bool reverse
);

using namespace MongoHistorian;

SampleKey::SampleKey() :
    m_timestamp(0),
    m_scale(0)
{

}

Sample::Sample() :
    m_timestamp(0)
{

}

Event::Event() :
    m_object(nullptr),
    m_owner(false),
    m_timestamp(0)
{

}

Event::~Event()
{
    if (m_object != nullptr)
    {
        if (m_owner == true)
        {
            CORTO_NULL_OWNER(corto_delete(m_object));
            m_object = nullptr;
        }
        else
        {
            corto_release(m_object);
        }
    }
}

std::size_t SampleKeyHasher::operator ()(const SampleKey &k) const
{
    typedef char byte_t;

    const std::size_t FNV_prime = 1099511628211;
    const std::size_t offset_basic = 14695981039346656037U;

    byte_t c = 0;

    std::size_t hash = offset_basic;

    const byte_t *it;
    //
    it = k.m_parent.c_str();
    while ((c = *it++) != '\0')
    {
        hash = (hash ^ c) * FNV_prime;
    }
    //
    it = k.m_id.c_str();
    while ((c = *it++) != '\0')
    {
        hash = (hash ^ c) * FNV_prime;
    }
    //
    it = k.m_type.c_str();
    while ((c = *it++) != '\0')
    {
        hash = (hash ^ c) * FNV_prime;
    }
    //
    it = (byte_t*)&k.m_timestamp;
    int size = sizeof(uint64_t);
    for (int i = 0; i < size; i++)
    {
        c = *it++;
        hash = (hash ^ c) * FNV_prime;
    }

    return hash;
}

bool SampleKeyEquals::operator ()(const SampleKey &lhs, const SampleKey &rhs) const
{
    bool retVal = false;
    char c1, c2;
    const char *s1, *s2;

    retVal = lhs.m_timestamp == rhs.m_timestamp &&
             lhs.m_parent.size() == rhs.m_parent.size();

    s1 = lhs.m_parent.c_str();
    s2 = rhs.m_parent.c_str();

    while (retVal && (c1 = *s1++) != '\0' && (c2 = *s2++) != '\0')
    {
        retVal = c1 == c2;
    }

    if (retVal)
    {
        retVal = lhs.m_id.size() == rhs.m_id.size();

        s1 = lhs.m_id.c_str();
        s2 = rhs.m_id.c_str();

        while (retVal && (c1 = *s1++) != '\0' && (c2 = *s2++) != '\0')
        {
            retVal = c1 == c2;
        }

        if (retVal)
        {
            retVal = lhs.m_type.size() == rhs.m_type.size();
            s1 = lhs.m_type.c_str();
            s2 = rhs.m_type.c_str();

            while (retVal && (c1 = *s1++) != '\0' && (c2 = *s2++) != '\0')
            {
                retVal = c1 == c2;
            }
        }
    }

    return retVal;
}

/* *******************************
 * HistoryCursor
 * *******************************/
HistoryCursor::HistoryCursor(mongocxx::cursor &&cursor,
                             mongocxx::cursor::iterator &&iter) :
    m_cursor(std::move(cursor)),
    m_iter(std::move(iter))
{

}

HistoryCursor::HistoryCursor(HistoryCursor &&cursor) :
    m_cursor(std::move(cursor.m_cursor)),
    m_iter(std::move(cursor.m_iter))
{

}

HistoryCursor::~HistoryCursor()
{

}

Data HistoryCursor::GetData()
{
    Data retVal;

    bsoncxx::document::view doc = *m_iter;
    element timestamp = doc["timestamp"];
    element value = doc["value"];

    retVal.m_timestamp = timestamp.get_date().value;
    retVal.m_data = value.get_utf8().value.to_string();

    return retVal;
}

bool HistoryCursor::HasNext()
{
    bool retVal = false;

    while (retVal == false && m_iter != m_cursor.end())
    {
        bsoncxx::document::view doc = *m_iter;
        element timestamp = doc["timestamp"];
        element value = doc["value"];

        if (timestamp && timestamp.type() == bsoncxx::type::k_date &&
            value && value.type() == bsoncxx::type::k_utf8)
        {
            retVal = true;
            break;
        }
        else
        {
            m_iter++;
        }
    }

    return retVal;
}

bool HistoryCursor::Next()
{
    bool retVal = false;

    if (m_iter != m_cursor.end())
    {
        m_iter++;
        retVal = true;
    }

    return retVal;
}

/* *******************************
 * Cursor
 * *******************************/
//
Cursor::Cursor(MongoClientPtr &&pClient,
               mongocxx::cursor &&cursor,
               mongocxx::cursor::iterator &&iter,
               std::string &&database,
               std::string &&collection,
               optional<std::chrono::milliseconds> &&start,
               optional<std::chrono::milliseconds> &&end,
               optional<uint64_t> &&skip,
               optional<uint64_t> &&limit,
               bool reverse) :
    m_pClient(std::move(pClient)),
    m_cursor(std::move(cursor)),
    m_iter(std::move(iter)),
    m_database(std::move(database)),
    m_collection(std::move(collection)),
    m_start(std::move(start)),
    m_end(std::move(end)),
    m_skip(std::move(skip)),
    m_limit(std::move(limit)),
    m_reverse(reverse)
{

}

Cursor::Cursor(Cursor &&cursor) :
    m_pClient(std::move(cursor.m_pClient)),
    m_cursor(std::move(cursor.m_cursor)),
    m_iter(std::move(cursor.m_iter)),
    m_database(std::move(cursor.m_database)),
    m_collection(std::move(cursor.m_collection)),
    m_start(std::move(cursor.m_start)),
    m_end(std::move(cursor.m_end)),
    m_skip(std::move(cursor.m_skip)),
    m_limit(std::move(cursor.m_limit))
{

}

Cursor::~Cursor()
{

}

Data Cursor::GetData()
{
    Data retVal;

    bsoncxx::document::view doc = *m_iter;

    element _id = doc["_id"];

    m_id = _id["id"].get_utf8().value.to_string();
    std::string type = _id["type"].get_utf8().value.to_string();

    retVal.m_name = m_id;
    retVal.m_type = type;

    return retVal;
}

HistoryCursor Cursor::GetHistoryCursor()
{
    auto cursor = MongoHistorySelect(m_pClient,
                                     m_database,
                                     m_collection,
                                     m_id,
                                     m_start,
                                     m_end,
                                     m_skip,
                                     m_limit,
                                     m_reverse);
    auto iter = cursor.begin();
    return HistoryCursor(std::move(cursor), std::move(iter));
}

bool Cursor::HasNext()
{
    bool retVal = false;

    while (retVal == false && m_iter != m_cursor.end())
    {
        bsoncxx::document::view doc = *m_iter;
        element _id = doc["_id"];
        element name = _id["id"];
        element type = _id["type"];
        if (name && name.type() == bsoncxx::type::k_utf8 &&
            type && type.type() == bsoncxx::type::k_utf8)
        {
            retVal = true;
        }
        else
        {
            m_iter++;
        }
    }

    return retVal;
}

bool Cursor::Next()
{
    bool retVal = false;

    if (m_iter != m_cursor.end())
    {
        m_iter++;
        retVal = true;
    }

    return retVal;
}

/* *******************************
 * MongoHistorian
 * *******************************/
/* PRIVATE: */
void CMongoHistorian::ProcessSamples()
{
    UniqueLock lock(m_eventMutex);
    EventMap events = std::move(m_eventBuffer);
    lock.unlock();

    for (EventMap::iterator iter = events.begin();
         iter != events.end();
         iter++)
    {
        Event &event = iter->second;

        SampleKey sampleKey = iter->first;
        sampleKey.m_scale = m_scale;
        sampleKey.m_timestamp = (event.m_timestamp/(60000*m_scale));

        if (event.m_object != nullptr)
        {
            corto_string str = corto_contentof(nullptr, "text/json", event.m_object);

            event.m_value = SAFE_STRING(str);
            if (event.m_owner == true)
            {
                CORTO_NULL_OWNER(corto_delete(event.m_object));
                event.m_object = nullptr;
            }
            else
            {
                corto_release(event.m_object);
                event.m_object = nullptr;
            }
        }

        Sample sample;
        sample.m_value = std::move(event.m_value);
        sample.m_timestamp = event.m_timestamp;

        m_sampleBuffer[sampleKey].push_back(std::move(sample));
    }
}

void CMongoHistorian::ProcessEvents()
{
    MongoClientPtr pClient = m_mongoPool.GetClient();

    try
    {
        for (BufferMap::iterator iter = m_sampleBuffer.begin();
             iter != m_sampleBuffer.end();
             iter++)
        {
            const SampleKey &sampleKey = iter->first;
            SampleSeq &samples = iter->second;
            //
            InitializeDocument(pClient, m_database, sampleKey);

            uint64_t keyTimestampScale = sampleKey.m_scale * 60000;

            bsoncxx::builder::stream::document updateBuilder;
            bsoncxx::builder::stream::document pushBuilder;

            for (SampleSeq::iterator it = samples.begin(); it != samples.end(); )
            {
                bsoncxx::builder::stream::array samplesArr;

                bool run = true;

                Sample *sample = &*it++;

                uint64_t currKey = (sample->m_timestamp/keyTimestampScale)%60;

                std::string key = "samples."+std::to_string(currKey)+".samples";
                while (run)
                {
                    samplesArr << open_document
                                << "timestamp" << ISODate(Milliseconds(sample->m_timestamp))
                                << "value" << sample->m_value
                                << close_document;
                    if (it != samples.end())
                    {
                        sample = &(*it);
                        uint64_t newKey = (sample->m_timestamp/keyTimestampScale)%60;
                        if (newKey == currKey)
                        {
                            it++;
                        }
                        else
                        {
                            run = false;
                        }
                    }
                    else
                    {
                        run = false;
                    }
                }

                pushBuilder << key << open_document
                            << "$each" << b_array{samplesArr.view()}
                            << close_document;
            }

            updateBuilder << "$push" << b_document{pushBuilder.view()};

            bsoncxx::builder::stream::document filterBuilder;
            filterBuilder << "id" << sampleKey.m_id
                          << "type" << sampleKey.m_type
                          << "timestamp" << ISODate(Minutes(sampleKey.m_timestamp * sampleKey.m_scale));

            mongocxx::collection coll = (*pClient)[m_database][sampleKey.m_parent];

            coll.update_one(filterBuilder.view(), updateBuilder.view());
        }
    }
    catch (std::exception &e)
    {
        corto_error("Failed to process Events [%s]", e.what());
    }
}

void CMongoHistorian::WorkerThread()
{
    //m_sampleRate
    std::chrono::system_clock::time_point lastUpdate = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point currTime;

    m_running = true;
    UniqueLock lock(m_threadMutex);
    while (m_done == false)
    {
        currTime = std::chrono::system_clock::now();
        ProcessSamples();
        uint64_t dif = std::chrono::duration_cast<Milliseconds>(currTime - lastUpdate).count();
        if (dif >= m_updateRate)
        {
            lastUpdate = currTime;
            ProcessEvents();
        }
        m_wakeUp.wait_for(lock, Milliseconds(m_sampleRate));
    }

    ProcessEvents();
    m_running = false;
}

void CMongoHistorian::InitializeDocument(MongoClientPtr &pClient,
                        std::string &database,
                        const SampleKey &key)
{
    uint64_t timestamp = key.m_timestamp * key.m_scale;

    const std::string &collection = key.m_parent;

    auto db = (*pClient)[database];

    if (m_expireAfterSeconds > 0 && db.has_collection(collection) == false)
    {
        bsoncxx::builder::stream::document indexBuilder;
        indexBuilder << "timestamp" << 1;

        mongocxx::options::index optionsIndex;
        optionsIndex.expire_after_seconds(m_expireAfterSeconds);

        db[collection].create_index(indexBuilder.view(), optionsIndex);
    }

    bsoncxx::builder::stream::document filterBuilder;
    filterBuilder << "id" << key.m_id
                  << "type" << key.m_type
                  << "timestamp" << ISODate(Minutes(timestamp));
    //
    mongocxx::collection coll = db[collection];

    auto found = coll.find_one(filterBuilder.view());
    if (!found)
    {
        // Insert document
        bsoncxx::builder::stream::document doc;
        doc << "id" << key.m_id
            << "type" << key.m_type
            << "timestamp" << ISODate(Minutes(timestamp));
        bsoncxx::builder::stream::array samplesArr;

        uint64_t startTime = timestamp * 60;          // seconds
        uint64_t endTime = startTime + 60 * key.m_scale;

        for (uint64_t i = startTime;
            i < endTime;
            i += key.m_scale)
        {
            samplesArr << open_document
                         << "timestamp" << ISODate(Seconds(i))
                         << "samples" << open_array << close_array
                         << close_document;
        }

        doc << "samples" << b_array{samplesArr.view()};

        coll.insert_one(doc.view());
    }
}

/* PUBLIC: */
void CMongoHistorian::Initialize(std::string user,
                std::string password,
                std::string hostaddr,
                std::string port,
                std::string database,
                uint64_t sampleRate,
                uint64_t updateRate,
                uint64_t expireAfterSeconds)
{
    m_mongoPool.Initialize(user, password, hostaddr, port);

    m_database = database;
    m_sampleRate = sampleRate;
    m_updateRate = updateRate;

    // if sample rate is < 500ms use 1 minute documents else use 1 hour documents
    m_scale = (sampleRate < 500) ? 1 : 60;

    m_expireAfterSeconds = expireAfterSeconds;

    if (m_expireAfterSeconds > 0)
    {
        bsoncxx::builder::stream::document indexBuilder;
        indexBuilder << "timestamp" << 1;
        mongocxx::options::modify_collection modifyOptions;
        modifyOptions.index( indexBuilder.view(), Seconds(m_expireAfterSeconds));
        MongoClientPtr pClient = m_mongoPool.GetClient();
        mongocxx::database db = (*pClient)[m_database];
        mongocxx::cursor collCursor = db.list_collections();
        for (bsoncxx::document::view doc : collCursor)
        {
            std::string name = doc["name"].get_utf8().value.to_string();
            db.modify_collection(name, modifyOptions);
        }
    }

    if (m_running == false)
    {
        m_thread = std::thread(&CMongoHistorian::WorkerThread, this);
        while(m_running == false)
        {
            std::this_thread::sleep_for(Microseconds(DEFAULT_WAIT_US));
        }
    }
}

void CMongoHistorian::Stop()
{
    UniqueLock lock(m_threadMutex);
    m_done = true;
    m_wakeUp.notify_all();
    lock.unlock();

    if (m_thread.joinable() == true)
    {
        m_thread.join();
    }
}

void CMongoHistorian::UpdateSample(std::string &parent,
                  std::string &id,
                  std::string &type,
                  std::string &value)
{
    StrToLower(parent);
    StrToLower(id);

    if (parent == ".")
    {
        parent = "/";
    }

    SampleKey sampleKey;
    sampleKey.m_parent = std::move(parent);
    sampleKey.m_id = std::move(id);
    sampleKey.m_type = std::move(type);

    LockGuard lock(m_eventMutex);
    Event &updateEvent = m_eventBuffer[sampleKey];

    if (updateEvent.m_object != nullptr)
    {
        if (updateEvent.m_owner == true)
        {
            CORTO_NULL_OWNER(corto_delete(updateEvent.m_object));

            updateEvent.m_object = nullptr;
        }
        else
        {
            corto_release(updateEvent.m_object);
            updateEvent.m_object = nullptr;
        }
    }

    updateEvent.m_value = std::move(value);
    auto timeStamp = std::chrono::system_clock::now();
    updateEvent.m_timestamp =  TO_MILLISECONDS(timeStamp).count();
}

void CMongoHistorian::UpdateSample(std::string &parent,
                  std::string &id,
                  std::string &type,
                  corto_object object)
{
    StrToLower(parent);
    StrToLower(id);

    if (parent == ".")
    {
        parent = "/";
    }

    SampleKey sampleKey;
    sampleKey.m_parent = std::move(parent);
    sampleKey.m_id = std::move(id);
    sampleKey.m_type = std::move(type);

    corto_object typeObj = corto_typeof(object);
    CopyCallbackHandler copyCb = CORTO_OLS_GET_COPY_CB(typeObj);

    LockGuard lock(m_eventMutex);
    Event &updateEvent = m_eventBuffer[sampleKey];

    if (updateEvent.m_object != nullptr && updateEvent.m_owner == false)
    {
        corto_release(updateEvent.m_object);
        updateEvent.m_object = nullptr;
    }

    if (copyCb != nullptr)
    {
        CORTO_NULL_OWNER(copyCb(&updateEvent.m_object, object));
        updateEvent.m_owner = true;
    }
    else
    {
        corto_warning("Unable to find copy callback for type=%s",
                      corto_fullpath(nullptr, typeObj));
        updateEvent.m_object = object;
        updateEvent.m_owner = false;
        corto_claim(updateEvent.m_object);
    }

    auto timeStamp = std::chrono::system_clock::now();
    updateEvent.m_timestamp =  TO_MILLISECONDS(timeStamp).count();
}

Cursor CMongoHistorian::GetCursor(std::string &parent,
                 std::string &expr,
                 optional<std::chrono::milliseconds> start,
                 optional<std::chrono::milliseconds> end,
                 optional<uint64_t> skip,
                 optional<uint64_t> limit,
                 bool reverse)
{
    if (parent == ".")
    {
        parent = "/";
    }

    StrToLower(parent);
    StrToLower(expr);
    std::string database = m_database;
    std::string collection = parent;

    bsoncxx::builder::stream::document filterBuilder;
    bsoncxx::builder::stream::document fieldsBuilder;

    fieldsBuilder << "_id"
                  << open_document
                     << "id" << "$id"
                     << "type" << "$type"
                  << close_document;

    mongocxx::pipeline stages;

    if (expr.find("*") == std::string::npos)
    {
        filterBuilder << "id" << expr;
        stages.match(filterBuilder.view());
    }
    else if (expr != "*")
    {
        filterBuilder << "id" << bsoncxx::types::b_regex(expr, "i");
        stages.match(filterBuilder.view());
    }

    stages.group(fieldsBuilder.view());

    MongoClientPtr pClient = m_mongoPool.GetClient();

    mongocxx::collection coll = (*pClient)[database][collection];

    mongocxx::cursor cursor = coll.aggregate(stages);
    mongocxx::cursor::iterator iter = cursor.begin();

    return Cursor(std::move(pClient),
                  std::move(cursor),
                  std::move(iter),
                  std::move(database),
                  std::move(collection),
                  std::move(start),
                  std::move(end),
                  std::move(skip),
                  std::move(limit),
                  reverse);
}

CMongoHistorian::CMongoHistorian() :
    m_sampleRate(0),
    m_updateRate(0),
    m_scale(0),
    m_expireAfterSeconds(0),
    m_done(false),
    m_running(false)
{

}

CMongoHistorian::~CMongoHistorian()
{

}

/* *******************************
 * static functions
 * *******************************/

static b_date ISODate(Minutes minutes)
{
    std::chrono::milliseconds millis = std::chrono::milliseconds(minutes);
    return b_date(millis.count());
}

static b_date ISODate(Milliseconds millis)
{
    return b_date(millis.count());
}


#define AND(lhs, rhs) "$and" \
    << open_array \
        << open_document << lhs << close_document \
        << open_document << rhs << close_document \
    << close_array

#define GTE(key, value) key \
    << open_document \
        << "$gte" \
        << value \
    << close_document

#define LTE(key, value) key \
    << open_document \
        << "$lte" \
        << value \
    << close_document

static mongocxx::cursor MongoHistorySelect(
    MongoClientPtr &pClient,
    std::string &database,
    std::string &collection,
    std::string &id,
    optional<std::chrono::milliseconds> start,
    optional<std::chrono::milliseconds> end,
    optional<uint64_t> skip,
    optional<uint64_t> limit,
    bool reverse
)
{
    bsoncxx::builder::stream::document filterHoursBuilder;
    bsoncxx::builder::stream::document filterMinutesBuilder;
    bsoncxx::builder::stream::document filterSecondsBuilder;
    bsoncxx::builder::stream::document filterPropertyBuilder;
    bsoncxx::builder::stream::document filterGroupBuilder;
    bsoncxx::builder::stream::document filterSortBuilder;

    // SELECT samples.samples.timestamp AS timestamp,
    //        samples.samples.value AS value
    //        samples.samples.type AS type

    filterPropertyBuilder << "samples.samples.timestamp" << 1
                          << "samples.samples.value" << 1;

    filterGroupBuilder << "_id" << "$samples.samples.timestamp"
                       << "timestamp" << open_document
                          << "$last" << "$samples.samples.timestamp"
                       << close_document
                       << "value" << open_document
                          << "$last" << "$samples.samples.value"
                       << close_document;

    // WHERE id = id
    filterHoursBuilder << "id" << id;
    if (start)
    {
        if (end)
        {
            // timestamp > start and timestamp  < end
            filterHoursBuilder << AND(GTE("timestamp", ISODate(start.value())),
                                      LTE("timestamp", ISODate(end.value())));
            filterMinutesBuilder << AND(GTE("samples.timestamp", ISODate(start.value())),
                                      LTE("samples.timestamp", ISODate(end.value())));
            filterSecondsBuilder << AND(GTE("samples.samples.timestamp", ISODate(start.value())),
                                      LTE("samples.samples.timestamp", ISODate(end.value())));
        }
        else
        {
            // timestamp > start
            filterHoursBuilder << GTE("timestamp", ISODate(start.value()));
            filterMinutesBuilder << GTE("samples.timestamp", ISODate(start.value()));
            filterSecondsBuilder << GTE("samples.samples.timestamp", ISODate(start.value()));
        }
    }
    else if (end)
    {
        // timestamp  < end
        filterHoursBuilder << LTE("timestamp", ISODate(end.value()));
        filterMinutesBuilder << LTE("samples.timestamp", ISODate(end.value()));
        filterSecondsBuilder << LTE("samples.samples.timestamp", ISODate(end.value()));
    }

    bool filterTime = start || end;

    if (reverse)
    {
        filterSortBuilder << "timestamp" << -1;
    }
    else
    {
        filterSortBuilder << "timestamp" << 1;
    }

    mongocxx::pipeline stages;
    stages.match(filterHoursBuilder.view());
    stages.unwind("$samples");

    if (filterTime)
    {
        stages.match(filterMinutesBuilder.view());
    }

    stages.unwind("$samples.samples");

    if (filterTime)
    {
        stages.match(filterSecondsBuilder.view());
    }

    stages.project(filterPropertyBuilder.view());
    stages.group(filterGroupBuilder.view());
    stages.sort(filterSortBuilder.view());

    if (limit)
    {
        if (skip)
        {
            limit.setValue(limit.value() + skip.value());
        }
        stages.limit(limit.value());
    }

    if (skip)
    {
        stages.skip(skip.value());
    }

    mongocxx::collection coll = (*pClient)[database][collection];

    return coll.aggregate(stages);
}

















///
