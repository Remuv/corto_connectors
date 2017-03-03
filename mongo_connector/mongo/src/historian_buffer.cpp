#include <recorto/mongo_connector/mongo/historian_buffer.h>
#include <recorto/mongo_connector/mongo/mongo_util.h>

#include <chrono>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/builder/core.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/stdx.hpp>

#define DEFAULT_WAIT_US 10

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

// static b_date ISODate(Hours hours)
// {
//     std::chrono::milliseconds millis = std::chrono::milliseconds(hours);
//     return b_date(millis.count());
// }

static b_date ISODate(Minutes minutes)
{
    std::chrono::milliseconds millis = std::chrono::milliseconds(minutes);
    return b_date(millis.count());
}

static b_date ISODate(Milliseconds millis)
{
    return b_date(millis.count());
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
    it = k.parent.c_str();
    while ((c = *it++) != '\0')
    {
        hash = (hash ^ c) * FNV_prime;
    }
    //
    it = k.id.c_str();
    while ((c = *it++) != '\0')
    {
        hash = (hash ^ c) * FNV_prime;
    }
    //
    it = k.type.c_str();
    while ((c = *it++) != '\0')
    {
        hash = (hash ^ c) * FNV_prime;
    }
    //
    it = (byte_t*)&k.timestamp;
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

    retVal = lhs.timestamp == rhs.timestamp &&
             lhs.parent.size() == rhs.parent.size();

    s1 = lhs.parent.c_str();
    s2 = rhs.parent.c_str();

    while (retVal && (c1 = *s1++) != '\0' && (c2 = *s2++) != '\0')
    {
        retVal = c1 == c2;
    }

    if (retVal)
    {
        retVal = lhs.id.size() == rhs.id.size();

        s1 = lhs.id.c_str();
        s2 = rhs.id.c_str();

        while (retVal && (c1 = *s1++) != '\0' && (c2 = *s2++) != '\0')
        {
            retVal = c1 == c2;
        }

        if (retVal)
        {
            retVal = lhs.type.size() == rhs.type.size();
            s1 = lhs.type.c_str();
            s2 = rhs.type.c_str();

            while (retVal && (c1 = *s1++) != '\0' && (c2 = *s2++) != '\0')
            {
                retVal = c1 == c2;
            }
        }
    }

    return retVal;
}

void CMongoHistorian::InitializeDocument(MongoClientPtr &pClient,
                                         std::string &database,
                                         const SampleKey &key)
{
    uint64_t timestamp = key.timestamp * key.scale;

    const std::string &collection = key.parent;

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
    filterBuilder << "id" << key.id
                  << "type" << key.type
                  << "timestamp" << ISODate(Minutes(timestamp));
    //
    mongocxx::collection coll = db[collection];

    auto found = coll.find_one(filterBuilder.view());
    if (!found)
    {
        // Insert document
        bsoncxx::builder::stream::document doc;
        doc << "id" << key.id
            << "type" << key.type
            << "timestamp" << ISODate(Minutes(timestamp));
        bsoncxx::builder::stream::array samplesArr;

        uint64_t startTime = timestamp * 60;          // seconds
        uint64_t endTime = startTime + 60 * key.scale;

        for (uint64_t i = startTime;
            i < endTime;
            i += key.scale)
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


void CMongoHistorian::ProcessEvent()
{
    UniqueLock lock(m_bufferMtx);
    BufferMap buffer = std::move(m_updateBuffer);
    lock.unlock();
    if (m_pMongoPool == nullptr)
    {
        return;
    }

    MongoClientPtr pClient = m_pMongoPool->GetClient();

    try
    {
        for (BufferMap::iterator iter = buffer.begin();
             iter != buffer.end();
             iter++)
        {
            const SampleKey &sampleKey = iter->first;
            SampleSeq &samples = iter->second;
            //
            InitializeDocument(pClient, m_database, sampleKey);

            uint64_t keyTimestampScale = sampleKey.scale * 60000;

            bsoncxx::builder::stream::document updateBuilder;
            bsoncxx::builder::stream::document pushBuilder;

            for (SampleSeq::iterator it = samples.begin(); it != samples.end(); )
            {
                bsoncxx::builder::stream::array samplesArr;

                bool run = true;

                Sample *sample = &*it++;

                uint64_t currKey = (sample->timestamp/keyTimestampScale)%60;

                std::string key = "samples."+std::to_string(currKey)+".samples";
                while (run)
                {
                    samplesArr << open_document
                                << "timestamp" << ISODate(Milliseconds(sample->timestamp))
                                << "value" << sample->value
                                << close_document;
                    if (it != samples.end())
                    {
                        sample = &(*it);
                        uint64_t newKey = (sample->timestamp/keyTimestampScale)%60;
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
            filterBuilder << "id" << sampleKey.id
                          << "type" << sampleKey.type
                          << "timestamp" << ISODate(Minutes(sampleKey.timestamp * sampleKey.scale));

            mongocxx::collection coll = (*pClient)[m_database][sampleKey.parent];

            coll.update_one(filterBuilder.view(), updateBuilder.view());
        }
    }
    catch (std::exception &e)
    {
        corto_error("Failed to process Events [%s]", e.what());
    }
}

void CMongoHistorian::ThreadStart()
{
    m_running = true;
    UniqueLock lock(m_threadMtx);
    while (m_done == false)
    {
        ProcessEvent();
        m_wakeUp.wait_for(lock, Milliseconds(m_period));
    }

    m_running = false;
}

void CMongoHistorian::Initialize(CMongoPool *pMongoPool,
                                 std::string database,
                                 uint64_t period,
                                 uint64_t scale,
                                 uint32_t expireAfterSeconds)
{
    LockGuard lock(m_threadMtx);
    m_expireAfterSeconds = expireAfterSeconds;
    m_database = database;
    m_pMongoPool = pMongoPool;
    m_period = period;
    m_scale  = scale;

    if (m_period != 0)
    {
        if (m_running == false)
        {
            m_thread = std::thread(&CMongoHistorian::ThreadStart, this);
            while (m_running == false)
            {
                std::this_thread::sleep_for(Microseconds(DEFAULT_WAIT_US));
            }
        }
    }

    if (m_expireAfterSeconds > 0)
    {
        bsoncxx::builder::stream::document indexBuilder;
        indexBuilder << "timestamp" << 1;

        mongocxx::options::modify_collection modifyOptions;
        modifyOptions.index( indexBuilder.view(), Seconds(m_expireAfterSeconds));

        MongoClientPtr pClient = m_pMongoPool->GetClient();

        mongocxx::database db = (*pClient)[m_database];
        mongocxx::cursor collCursor = db.list_collections();

        for (bsoncxx::document::view doc : collCursor)
        {
            std::string name = doc["name"].get_utf8().value.to_string();
            db.modify_collection(name, modifyOptions);
        }
    }
}

void CMongoHistorian::Stop()
{
    UniqueLock lock(m_threadMtx);
    m_done = true;
    m_wakeUp.notify_all();
    lock.unlock();

    if (m_thread.joinable() == true)
    {
        m_thread.join();
    }
}

void CMongoHistorian::UpdateSample(std::string &&parent,
                                   std::string &&id,
                                   std::string &&type,
                                   std::string &&value,
                                   uint64_t timestamp)
{
    if (m_running == true)
    {
        SampleKey sampleKey;
        sampleKey.parent = std::move(parent);
        sampleKey.id = std::move(id);
        sampleKey.type = std::move(type);
        sampleKey.scale = m_scale;
        sampleKey.timestamp = (timestamp/(60000*m_scale));

        Sample sample;
        sample.value = std::move(value);
        sample.timestamp = timestamp;

        LockGuard lock(m_bufferMtx);
        m_updateBuffer[sampleKey].push_back(std::move(sample));
    }
    else
    {
        InsertOne(parent, id, type, value, timestamp);
    }
}

void CMongoHistorian::InsertOne(std::string &parent,
                                std::string &id,
                                std::string &type,
                                std::string &value,
                                uint64_t milliseconds)
{
    MongoClientPtr pClient = m_pMongoPool->GetClient();

    SampleKey sampleKey;
    sampleKey.parent = parent;
    sampleKey.id = id;
    sampleKey.type = type;
    sampleKey.timestamp = (milliseconds/(60000*m_scale));

    InitializeDocument(pClient, m_database, sampleKey);

    uint64_t keyTimestampScale = sampleKey.scale * 60000;
    uint64_t currKey = (milliseconds/keyTimestampScale)%60;

    std::string key = "samples."+std::to_string(currKey)+".samples";
    try
    {
        // Add sample
        bsoncxx::builder::stream::document filterBuilder;
        filterBuilder << "id" << id
                      << "type" << type
                      << "timestamp" << ISODate(Minutes(sampleKey.timestamp*m_scale));

        bsoncxx::builder::stream::document updateBuilder;
        updateBuilder << "$push"
                      << open_document
                         << key
                         << open_document
                            << "timestamp" << ISODate(Milliseconds(milliseconds))
                            << "value" << value
                         << close_document
                      << close_document;
        //
        auto coll = (*pClient)[m_database][parent];
        coll.update_one(filterBuilder.view(), updateBuilder.view());
    }
    catch (std::exception &e)
    {
        corto_error("Failed to insert sample [%s], error = %s", key.c_str(), e.what());
    }
}

CMongoHistorian::CMongoHistorian() :
    m_pMongoPool(nullptr),
    m_done(false),
    m_running(false)
{

}

CMongoHistorian::~CMongoHistorian()
{

}



















///
