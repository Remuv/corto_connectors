
#include "sync_adapter.h"

CSyncAdapter::CSyncAdapter(std::string topic) :
        m_pDataPublisher(nullptr),
        m_pDataSubscriber(nullptr),
        m_pRequestPublisher(nullptr),
        m_pRequestSubscriber(nullptr)
{
    m_ddsTopic = topic;
}

CSyncAdapter::~CSyncAdapter()
{

}

bool CSyncAdapter::SetUpDataPublisher(CCortoRequestSubscriber::DataNotifyCallback callback)
{
    m_pDataPublisher = new CCortoDataPublisher("StoreSync_"+m_ddsTopic);
    m_pRequestSubscriber = new CCortoRequestSubscriber("StoreRequest_"+m_ddsTopic);

    if (m_pDataPublisher->Initialize() == false || m_pRequestSubscriber->Initialize() == false)
    {
        delete m_pDataPublisher;
        delete m_pRequestSubscriber;
        return false;
    }
    CCortoRequestSubscriber::NewDataDelegate delegate(shared_from_this(), callback);
    m_pRequestSubscriber->RegisterNewDataSubscriber(delegate);
    return true;
}

bool CSyncAdapter::SetUpDataSubscriber(CCortoDataSubscriber::DataNotifyCallback callback)
{
    m_pDataSubscriber = new CCortoDataSubscriber("StoreSync_"+m_ddsTopic);
    m_pRequestPublisher = new CCortoRequestPublisher("StoreRequest_"+m_ddsTopic);

    if (m_pDataSubscriber->Initialize() == false || m_pRequestPublisher->Initialize() == false)
    {
        delete m_pDataSubscriber;
        delete m_pRequestPublisher;
        return false;
    }
    CCortoDataSubscriber::NewDataDelegate delegate( shared_from_this(), callback);
    m_pDataSubscriber->RegisterNewDataSubscriber(delegate);
    return true;
}

void CSyncAdapter::SendData(std::string name, std::string type, std::string value)
{
    if (m_pDataPublisher == nullptr)
    {
        return;
    }
    Corto::Data data(name, type, value);
    core::InstanceHandle handler = m_dataHandlers[data.name()];
    if (handler.is_nil())
    {
        handler = m_pDataPublisher->RegisterInstance(data);
        m_dataHandlers[data.name()] = handler;
    }
    m_pDataPublisher->Write(data, handler);
}

void CSyncAdapter::SendRequest(std::string name, std::string type, std::string value)
{
    if (m_pRequestPublisher == nullptr)
    {
        return;
    }
    Corto::Request request(name, type, value);
    core::InstanceHandle handler = m_requestHandlers[request.name()];

    if (handler.is_nil())
    {
        handler = m_pRequestPublisher->RegisterInstance(request);
        m_requestHandlers[request.name()] = handler;
    }
    m_pRequestPublisher->Write(request, handler);
}

void CSyncAdapter::Close()
{
    if (m_pRequestSubscriber != nullptr)
    {
        m_pRequestSubscriber->UnregisterNewDataSubscriber(shared_from_this());
        delete m_pRequestSubscriber;
        m_pRequestSubscriber = nullptr;
    }

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

    if (m_pRequestPublisher != nullptr)
    {
        HandlerMap::iterator handlerIt;
        for (handlerIt = m_requestHandlers.begin();
             handlerIt != m_requestHandlers.end();
             handlerIt++)
        {
            m_pRequestPublisher->UnregisterInstance(handlerIt->second);
        }
        delete m_pRequestPublisher;
        m_pRequestPublisher = nullptr;
    }
}
