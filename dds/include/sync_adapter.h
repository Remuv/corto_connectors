#ifndef __CORTO_DDS_SYNC_ADAPTER_H__
#define __CORTO_DDS_SYNC_ADAPTER_H__

#include <recorto/dds/dds.h>
#include <functional>
#include <memory>
#include <mutex>

class CSyncAdapter : public std::enable_shared_from_this<CSyncAdapter>
{
public:
    typedef CCortoDataSubscriber::Sample Sample;
    typedef CCortoDataSubscriber::SampleSeq SampleSeq;
    typedef std::vector<std::string> ParamVector;
    //typedef CCortoDataSubscriber::ParamVector ParamVector;

    typedef CCortoDataSubscriber::DataNotifyCallback DataNotifyCallback;

private:

    class Event {
    public:
        enum Type {
            UPDATE,
            DELETE
        };
        Type        m_event;

        std::string m_parent;
        std::string m_name;
        std::string m_type;
        std::string m_value;
    };

    typedef std::map<std::string, dds::core::InstanceHandle> HandlerMap;
    typedef std::map<std::string, Event> EventMap;

    //CRemuvUuid  m_uuid;
    std::string m_uuid;
    std::string m_ddsTopic;

    CCortoDataPublisher  *m_pDataPublisher;
    CCortoDataSubscriber *m_pDataSubscriber;

    HandlerMap m_dataHandlers;

    uint64_t      m_taskId;

    std::mutex m_mtx;
    EventMap   m_eventBuffer;

    void ProcessEvent();
    bool SendDataImpt(std::string &type, std::string &parent, std::string &name, std::string &value);
    bool UnregisterDataImpt(std::string &parent, std::string &name);

public:
    CSyncAdapter(std::string topic);
    ~CSyncAdapter();

    bool Initialize(DataNotifyCallback newDataCallback,
                    DataNotifyCallback disposedDataCallback,
                    uint64_t updateRate);


    bool SendData(std::string &type, std::string &parent, std::string &name, std::string &value);
    bool UnregisterData(std::string &parent, std::string &name);

    bool Query(SampleSeq &sampleSeq, std::string expression, ParamVector params);

    bool Query(SampleSeq &sampleSeq, std::string expression);

    void Close();
};

#endif
