#ifndef __MONGO_HISTORIAN_BUFFER_H__
#define __MONGO_HISTORIAN_BUFFER_H__

#include <recorto/mongo_connector/mongo/mongo_util.h>
#include <mongocxx/cursor.hpp>

#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <condition_variable>

namespace MongoHistorian
{
    struct SampleKey
    {
        std::string m_parent;
        std::string m_id;
        std::string m_type;
        uint64_t    m_timestamp;
        uint64_t    m_scale;   // 1 = Minutes document, 60 = Hours Documents

        SampleKey();
    };

    struct Sample
    {
        std::string  m_value;
        uint64_t     m_timestamp;
        Sample();
    };

    struct Event
    {
        std::string  m_value;
        corto_object m_object;
        uint64_t     m_timestamp;

        Event();
        ~Event();
    };

    struct SampleKeyHasher
    {
        std::size_t operator()(const SampleKey &k) const;
    };

    struct SampleKeyEquals
    {
        bool operator()(const SampleKey &lhs, const SampleKey &rhs) const;
    };

    struct Data
    {
        std::string m_name;
        std::string m_type;
        std::string m_data;
        uint64_t    m_timestamp;
    };

    class HistoryCursor
    {
     private:
        friend class Cursor;

        mongocxx::cursor           m_cursor;
        mongocxx::cursor::iterator m_iter;

        HistoryCursor(mongocxx::cursor &&cursor,
                      mongocxx::cursor::iterator &&iter);

     public:
        HistoryCursor(HistoryCursor &&cursor);
        ~HistoryCursor();

        Data GetData();

        bool HasNext();

        bool Next();
    };

    class Cursor
    {
     private:
        friend class CMongoHistorian;

        MongoClientPtr                      m_pClient;
        mongocxx::cursor                    m_cursor;
        mongocxx::cursor::iterator          m_iter;

        std::string                         m_database;
        std::string                         m_collection;
        optional<std::chrono::milliseconds> m_start;
        optional<std::chrono::milliseconds> m_end;
        optional<uint64_t>                  m_skip;
        optional<uint64_t>                  m_limit;
        bool                                m_reverse;

        std::string                         m_id;

        Cursor(MongoClientPtr &&pClient,
               mongocxx::cursor &&cursor,
               mongocxx::cursor::iterator &&iter,
               std::string &&database,
               std::string &&collection,
               optional<std::chrono::milliseconds> &&start,
               optional<std::chrono::milliseconds> &&end,
               optional<uint64_t> &&skip,
               optional<uint64_t> &&limit,
               bool reverse);

     public:
        Cursor(Cursor &&c);
        ~Cursor();

        Data GetData();

        HistoryCursor GetHistoryCursor();

        bool HasNext();

        bool Next();
    };

    class CMongoHistorian
    {
    private:
        typedef std::vector<Sample> SampleSeq;
        typedef std::unordered_map<SampleKey, SampleSeq, SampleKeyHasher, SampleKeyEquals> BufferMap;
        typedef std::unordered_map<SampleKey, Event, SampleKeyHasher, SampleKeyEquals> EventMap;

        typedef std::lock_guard<std::mutex>  LockGuard;
        typedef std::unique_lock<std::mutex> UniqueLock;


        std::mutex m_eventMutex;
        EventMap   m_eventBuffer;
        BufferMap  m_sampleBuffer;

        std::string m_database;
        uint64_t    m_sampleRate;
        uint64_t    m_updateRate;
        uint64_t    m_scale;
        uint64_t    m_expireAfterSeconds;

        CMongoPool  m_mongoPool;

        std::mutex              m_threadMutex;
        std::thread             m_thread;
        std::condition_variable m_wakeUp;
        volatile sig_atomic_t   m_done;
        volatile sig_atomic_t   m_running;

        void ProcessSamples();
        void ProcessEvents();
        void WorkerThread();

        void InitializeDocument(MongoClientPtr &pClient, std::string &database, const SampleKey &key);

    public:
        void Initialize(std::string user,
                        std::string password,
                        std::string hostaddr,
                        std::string port,
                        std::string database,
                        uint64_t sampleRate,
                        uint64_t updateRate,
                        uint64_t expireAfterSeconds);

        void Stop();

        void UpdateSample(std::string &parent,
                          std::string &id,
                          std::string &type,
                          std::string &value);

        void UpdateSample(std::string &parent,
                          std::string &id,
                          std::string &type,
                          corto_object object);

        Cursor GetCursor(std::string &parent,
                         std::string &expr,
                         optional<std::chrono::milliseconds> start,
                         optional<std::chrono::milliseconds> end,
                         optional<uint64_t> skip,
                         optional<uint64_t> limit,
                         bool reverse);

        CMongoHistorian();
        ~CMongoHistorian();
    };
}
#endif
