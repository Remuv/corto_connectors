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

static b_date ISODate(Hours hours)
{
    std::chrono::milliseconds millis = std::chrono::milliseconds(hours);
    return b_date(millis.count());
}

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
    it = (byte_t*)&k.hour;
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

    retVal = lhs.hour == rhs.hour &&
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


void InitializeDocument(MongoClientPtr &pClient,
                        std::string &database,
                        const SampleKey &key)
{
    uint64_t hours = key.hour;
    const std::string &collection = key.parent;

    bsoncxx::builder::stream::document filterBuilder;
    filterBuilder << "id" << key.id
                  << "type" << key.type
                  << "timestamp" << ISODate(Hours(hours));
    //
    mongocxx::collection coll = (*pClient)[database][collection];

    auto found = coll.find_one(filterBuilder.view());
    if (!found)
    {
        // Insert document
        bsoncxx::builder::stream::document doc;
        doc << "id" << key.id
            << "type" << key.type
            << "timestamp" << ISODate(Hours(hours));
        bsoncxx::builder::stream::array minutesArray;

        uint64_t minutes = hours * 60;
        for (int i = 0; i < 60; i++)
        {
            minutesArray << open_document
                         << "timestamp" << ISODate(Minutes(minutes+i))
                         << "seconds" << open_array << close_array
                         << close_document;
        }

        doc << "minutes" << b_array{minutesArray.view()};

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

    for (BufferMap::iterator iter = buffer.begin();
         iter != buffer.end();
         iter++)
    {
        const SampleKey &sampleKey = iter->first;
        SampleSeq &samples = iter->second;
        //
        InitializeDocument(pClient, m_database, sampleKey);

        bsoncxx::builder::stream::document updateBuilder;
        bsoncxx::builder::stream::document pushBuilder;

        for (SampleSeq::iterator it = samples.begin(); it != samples.end(); )
        {
            bsoncxx::builder::stream::array samplesSecs;

            bool run = true;

            Sample *sample = &*it++;

            uint64_t minutesKey = (sample->timestamp/60000)%60;

            std::string key = "minutes."+std::to_string(minutesKey)+".seconds";
            while (run)
            {
                samplesSecs << open_document
                            << "timestamp" << ISODate(Milliseconds(sample->timestamp))
                            << "value" << sample->value
                            << close_document;
                if (it != samples.end())
                {
                    sample = &(*it);
                    uint64_t newKey = (sample->timestamp/60000)%60;
                    if (newKey == minutesKey)
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
                        << "$each" << b_array{samplesSecs.view()}
                        << close_document;
        }

        updateBuilder << "$push" << b_document{pushBuilder.view()};

        bsoncxx::builder::stream::document filterBuilder;
        filterBuilder << "id" << sampleKey.id
                      << "type" << sampleKey.type
                      << "timestamp" << ISODate(Hours(sampleKey.hour));

        mongocxx::collection coll = (*pClient)[m_database][sampleKey.parent];

        coll.update_one(filterBuilder.view(), updateBuilder.view());
    }
}

void CMongoHistorian::ThreadStart()
{
    m_running = true;
    UniqueLock lock(m_threadMtx);
    while (m_done == false)
    {
        ProcessEvent();
        m_wakeUp.wait_for(lock, Microseconds(m_period));
    }

    m_running = false;
}

void CMongoHistorian::Initialize(CMongoPool *pMongoPool, std::string database, uint64_t period)
{
    LockGuard lock(m_threadMtx);
    m_database = database;
    m_pMongoPool = pMongoPool;
    m_period = period;

    if (m_running == false)
    {
        m_thread = std::thread(&CMongoHistorian::ThreadStart, this);
        while (m_running == false)
        {
            std::this_thread::sleep_for(Microseconds(DEFAULT_WAIT_US));
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
    SampleKey sampleKey;
    sampleKey.parent = std::move(parent);
    sampleKey.id = std::move(id);
    sampleKey.type = std::move(type);
    sampleKey.hour = (timestamp/3600000);

    Sample sample;
    sample.value = std::move(value);
    sample.timestamp = timestamp;

    LockGuard lock(m_bufferMtx);
    m_updateBuffer[sampleKey].push_back(std::move(sample));
}

void CMongoHistorian::InsertOne(std::string &parent,
                                std::string &id,
                                std::string &type,
                                std::string &value,
                                uint64_t milliseconds)
{
    MongoClientPtr pClient = m_pMongoPool->GetClient();

    uint64_t hours = (milliseconds/3600000);

    try
    {
        bsoncxx::builder::stream::document filterBuilder;
        filterBuilder << "id" << id
                      << "type" << type
                      << "timestamp" << ISODate(std::chrono::hours(hours));

        mongocxx::collection coll = (*pClient)[m_database][parent];

        auto found = coll.find_one(filterBuilder.view());
        if (!found)
        {
            // Insert document
            bsoncxx::builder::stream::document doc;
            doc << "id" << id
                << "type" << type
                << "timestamp" << ISODate(std::chrono::hours(hours));
            bsoncxx::builder::stream::array minutesArray;

            uint64_t minutes = hours * 60;
            for (int i = 0; i < 60; i++)
            {
                minutesArray << open_document
                             << "timestamp" << ISODate(std::chrono::minutes(minutes+i))
                             << "seconds" << open_array << close_array
                             << close_document;
            }

            doc << "minutes" << b_array{minutesArray.view()};

            coll.insert_one(doc.view());
        }

        uint64_t minutes = milliseconds/60000;
        std::string key = "minutes."+std::to_string(minutes%60)+".seconds";

        // Add sample
        bsoncxx::builder::stream::document updateBuilder;
        updateBuilder << "$push"
                      << open_document
                         << key
                         << open_document
                            << "timestamp" << ISODate(std::chrono::milliseconds(milliseconds))
                            << "value" << value
                         << close_document
                      << close_document;

        coll.update_one(filterBuilder.view(), updateBuilder.view());
    }
    catch(std::exception &e)
    {
        corto_error("%s", e.what());
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
