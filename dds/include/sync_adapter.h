#ifndef __CORTO_DDS_SYNC_ADAPTER_H__
#define __CORTO_DDS_SYNC_ADAPTER_H__

#include <recorto/dds/dds.h>
#include <functional>
#include <memory>

class CSyncAdapter : public std::enable_shared_from_this<CSyncAdapter>
{
private:
    typedef std::map<std::string, dds::core::InstanceHandle> HandlerMap;

    std::string m_ddsTopic;

    CCortoDataPublisher *m_pDataPublisher;
    CCortoDataSubscriber *m_pDataSubscriber;

    CCortoRequestPublisher *m_pRequestPublisher;
    CCortoRequestSubscriber *m_pRequestSubscriber;

    HandlerMap m_dataHandlers;
    HandlerMap m_requestHandlers;

public:
    CSyncAdapter(std::string topic);

    ~CSyncAdapter();

    bool SetUpDataPublisher(CCortoRequestSubscriber::DataNotifyCallback callback);

    bool SetUpDataSubscriber(CCortoDataSubscriber::DataNotifyCallback callback);

    bool SendData(std::string type, std::string parent, std::string name, std::string value);

    void SendRequest(std::string name, std::string type, std::string value);

    void Close();
};

#endif
