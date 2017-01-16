#ifndef __CORTO_DDS_SYNC_ADAPTER_H__
#define __CORTO_DDS_SYNC_ADAPTER_H__

#include <recorto/dds/dds.h>
#include <functional>
#include <memory>

class CSyncAdapter : public std::enable_shared_from_this<CSyncAdapter>
{
public:
    typedef CCortoDataSubscriber::Sample Sample;
    typedef CCortoDataSubscriber::SampleSeq SampleSeq;
    typedef std::vector<std::string> ParamVector;
    //typedef CCortoDataSubscriber::ParamVector ParamVector;

    typedef CCortoDataSubscriber::DataNotifyCallback DataNotifyCallback;

private:
    typedef std::map<std::string, dds::core::InstanceHandle> HandlerMap;

    //CRemuvUuid  m_uuid;
    std::string m_uuid;
    std::string m_ddsTopic;

    CCortoDataPublisher  *m_pDataPublisher;
    CCortoDataSubscriber *m_pDataSubscriber;

    HandlerMap m_dataHandlers;

public:
    CSyncAdapter(std::string topic);
    ~CSyncAdapter();

    bool Initialize(DataNotifyCallback newDataCallback,
                    DataNotifyCallback disposedDataCallback);


    bool SendData(std::string type, std::string parent, std::string name, std::string value);
    bool UnregisterData(std::string parent, std::string name);

    bool Query(SampleSeq &sampleSeq, std::string expression, ParamVector params);

    bool Query(SampleSeq &sampleSeq, std::string expression);

    void Close();
};

#endif
