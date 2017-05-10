#ifndef __CORTO_DDS_SYNC_ADAPTER_H__
#define __CORTO_DDS_SYNC_ADAPTER_H__

#include <recorto/dds/dds.h>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <thread>

#include <common/base/timer_instance.h>

struct StringHasher
{
    std::size_t operator()(const std::string &k) const;
};

struct StringEquals
{
    bool operator()(const std::string &lhs, const std::string &rhs) const;
};

class CSyncAdapter : public std::enable_shared_from_this<CSyncAdapter>
{
public:
    typedef CCortoDataSubscriber::Sample Sample;
    typedef CCortoDataSubscriber::SampleSeq SampleSeq;
    typedef std::vector<std::string> ParamVector;
    //typedef CCortoDataSubscriber::ParamVector ParamVector;

    typedef CCortoDataSubscriber::DataNotifyCallback DataNotifyCallback;
    typedef dds::core::InstanceHandle DdsInstanceHandle;

private:
    typedef std::unordered_set<std::string> ConnectionSet;
    // Objects On DDS Object Store
    struct ObjectData
    {
        ConnectionSet m_subscribers;

        DdsInstanceHandle m_handle;
        Corto::Data       m_data;
        bool              m_owner;
    };
    // Corto Events
    class Event
    {
     public:
        enum Type
        {
            NONE,
            CREATE,
            UPDATE,
            DELETE
        };
        
        Event();
        ~Event();

        Type         m_event;
        Corto::Data  m_data;
        corto_object m_object;
        bool         m_owner;
    };

    template<typename T>
    using RinMap = std::unordered_map<std::string, T, StringHasher, StringEquals>;

    typedef RinMap<std::string>          CacheMap;
    typedef RinMap<ObjectData>           ObjectMap;
    typedef RinMap<Event>                EventMap;
    typedef RinMap<int32_t>              RequestMap;
    typedef RinMap<DdsInstanceHandle>    InstanceMap;

    typedef std::lock_guard<std::mutex>  LockGuard;
    typedef std::unique_lock<std::mutex> UniqueLock;


    CCortoDataPublisher  *m_pDataPublisher;
    CCortoDataSubscriber *m_pDataSubscriber;

    CCortoMessagePublisher  *m_pMessagePublisher;
    CCortoMessageSubscriber *m_pMessageSubscriber;

    reTimer::CTimerFactory m_taskFactory;
    //CRemuvUuid  m_uuid;
    uint64_t    m_taskId;
    std::string m_ddsTopic;     // DDS TOPIC
    std::string m_uuid;         // UUID

    std::mutex  m_ebMtx;        // Event Buffer Mutex
    EventMap    m_eventBuffer;  // Events Buffer

    std::mutex    m_ddsMtx;        //
    ConnectionSet m_ddsConnectors; // Active Clients
    ObjectMap     m_objects;       // InstanceHandle map and subscribers count
    CacheMap      m_updateCache;   // Update Event Buffer (no subscribed instances)

    std::mutex  m_requestMtx;
    RequestMap  m_requests;
    uint64_t    m_lastID;         // last request id

    std::mutex  m_instancesMtx;
    InstanceMap m_instances;

    void ProcessEvent();

    bool CreateData(Event &event);
    bool UpdateData(Event &event);
    bool DeleteData(Event &event);
    void DisposeData(const Corto::Data &data);

    void SendUpdates();

    DdsInstanceHandle SendMessage(const std::string &dest, const std::string &id, uint8_t type);
    void DisposeMessage(const std::string &dest, const std::string &id);

    void OnCortoMessage(CCortoMessageSubscriber::Sample &sample);
    void OnCortoMessageDispose(CCortoMessageSubscriber::Sample &sample);


public:
    CSyncAdapter(std::string topic);
    ~CSyncAdapter();

    bool Initialize(DataNotifyCallback newDataCallback,
                    DataNotifyCallback disposedDataCallback,
                    uint64_t updateRate);

    bool CreateData(std::string &type,
                    std::string &parent,
                    std::string &id,
                    std::string &name,
                    std::string &value);
    bool CreateData(std::string &type,
                    std::string &parent,
                    std::string &id,
                    std::string &name,
                    corto_object object);

    bool UpdateData(std::string &type,
                    std::string &parent,
                    std::string &id,
                    std::string &name,
                    std::string &value);
    bool UpdateData(std::string &type,
                    std::string &parent,
                    std::string &id,
                    std::string &name,
                    corto_object object);

    bool DeleteData(std::string &parent, std::string &id);

    bool SubscribeData(std::string &parent, std::string &expr);
    bool UnsubscribeData(std::string &parent, std::string &expr);

    bool Query(SampleSeq &sampleSeq,
               std::string &parent,
               std::string &id,
               bool latest);

    void Close();
};

#endif
