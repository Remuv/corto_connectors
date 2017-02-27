#ifndef __MONGO_HISTORIAN_BUFFER_H__
#define __MONGO_HISTORIAN_BUFFER_H__

#include <recorto/mongo_connector/mongo/mongo_util.h>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <condition_variable>

struct SampleKey {
    std::string parent;
    std::string id;
    std::string type;
    uint64_t    hour;
};

struct Sample {
    std::string value;
    uint64_t    timestamp;
};

struct SampleKeyHasher
{
    std::size_t operator()(const SampleKey &k) const;
};

struct SampleKeyEquals
{
    bool operator()(const SampleKey &lhs, const SampleKey &rhs) const;
};

class CMongoHistorian
{
private:
    typedef std::vector<Sample> SampleSeq;
    typedef std::unordered_map<SampleKey, SampleSeq, SampleKeyHasher, SampleKeyEquals> BufferMap;
    typedef std::lock_guard<std::mutex>  LockGuard;
    typedef std::unique_lock<std::mutex> UniqueLock;

    std::mutex m_bufferMtx;
    BufferMap  m_updateBuffer;

    //
    CMongoPool             *m_pMongoPool;
    std::string             m_database;
    uint64_t                m_period;
    std::mutex              m_threadMtx;
    std::thread             m_thread;
    std::condition_variable m_wakeUp;

    volatile sig_atomic_t   m_done;
    volatile sig_atomic_t   m_running;

    void ProcessEvent();

    void ThreadStart();

public:
    void Initialize(CMongoPool *pMongoPool, std::string database, uint64_t period);
    void Stop();

    void UpdateSample(std::string &&parent,
                      std::string &&id,
                      std::string &&type,
                      std::string &&value,
                      uint64_t timestamp);
    void InsertOne(std::string &parent,
                   std::string &id,
                   std::string &type,
                   std::string &value,
                   uint64_t timestamp);

    CMongoHistorian();
    ~CMongoHistorian();
};
#endif
