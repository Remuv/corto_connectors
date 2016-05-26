/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/dds/dds.h>

/* $header() */


class CSyncAdapter : std::shared_from_this<CSyncAdapter>
{
private:
    std::string m_ddsTopic;
    dds_Connector *m_pConnector;

    CCortoDataPublisher *m_pDataPublisher;
    CCortoDataSubscriber *m_pDataSubscriber;

    CCortoRequestPublisher *m_pRequestPublisher;
    CCortoRequestSubscriber *m_pRequestSubscriber;

    core::InstanceHandle m_dataHandler;
    core::InstanceHandle m_requestHandler;

public:
    CSyncAdapter(dds_Connector *connector, std::string topic) :
            m_pDataSubscriber(nullptr),
            m_pDataPublisher(nullptr),
            m_pRequestSubscriber(nullptr),
            m_pRequestPublisher(nullptr)
    {
        m_pConnector = m_pConnector;
        m_ddstopic = topic;
    }

    ~CSyncAdapter()
    {

    }

    bool SetUpDataPublisher()
    {
        m_pDataPublisher = new CCortoDataPublisher("StoreSync_"+m_ddsTopic);
        m_pRequestSubscriber = new CCortoRequestSubscriber("StoreRequest_"+m_ddsTopic);

        if (m_pDataPublisher->Initialize() == false || m_pRequestSubscriber->Initialize() == false)
        {
            delete m_pDataPublisher;
            delete m_pRequestSubscriber;
            return false;
        }
        CCortoRequestSubscriber::NewDataDelegate delegate(getptr(),
                                     [&] (CCortoRequestSubscriber::Sample &sample){
                                         dds_Connector_OnRequest(m_pConnector, sample);
                                     });
        subscriber->RegisterNewDataSubscriber(delegate);
        return true;
    }

    bool SetUpDataSubscriber()
    {
        m_pDataSubscriber = new CCortoDataSubscriber("StoreSync_"+m_ddsTopic);
        m_pRequestPublisher = new CCortoRequestPublisher("StoreRequest_"+m_ddsTopic);

        if (m_pDataSubscriber->Initialize() == false || m_pRequestPublisher->Initialize() == false)
        {
            delete subscriber;
            delete publisher;
            return false;
        }
        CCortoDataSubscriber::NewDataDelegate delegate( getptr(),
                                     [&] (CCortoDataSubscriber::Sample &sample){
                                         dds_Connector_OnData(m_pConnector, sample);
                                     });
        m_pDataSubscriber->RegisterNewDataSubscriber(delegate);
        return false;
    }

    SendData(std::string name, std::string type, std::value)
    {

    }

    SendRequest(std::string name, std::string type, std::value)
    {

    }

}




#define cortoSharedPtr_int(obj) ((std::shared_ptr<int>*)obj)
#define cortoData_publisher(obj) ((CCortoDataPublisher*)obj)
#define cortoData_subscriber(obj) ((CCortoDataSubscriber*)obj)
#define cortoRequest_publisher(obj) ((CCortoRequestPublisher*)obj)
#define cortoRequest_subscriber(obj) ((CCortoRequestSubscriber*)obj)
#define cortoCore_instanceHandle(obj) ((core::InstanceHandle*)obj)

#define NULLWORD 0

corto_void dds_Connector_OnRequest(dds_Connector _this, CCortoRequestSubscriber::Sample &sample)
{
    corto_trace("OnRequest");

}

corto_void dds_Connector_OnData(dds_Connector _this, CCortoDataSubscriber::Sample &sample)
{
    corto_trace("OnData");

}

corto_void dds_Connector_SendData(dds_Connector _this, corto_string data, corto_string type, corto_string value)
{


}




/* $end */

corto_int16 _dds_Connector_construct(
    dds_Connector _this)
{
/* $begin(corto/dds/Connector/construct) */
    _this->ptr = NULLWORD;
    _this->publisher = NULLWORD;
    _this->subscriber = NULLWORD;
    _this->request_publisher = NULLWORD;
    _this->request_subscriber = NULLWORD;
    _this->data_handler = NULLWORD;
    _this->request_handler = NULLWORD;

    std::string syncSubTopic = "StoreSync_";
    std::string requestSubTopic = "StoreRequest_";

    std::shared_ptr<int> *sp = new std::shared_ptr<int>(new int(10));
    _this->ptr = (corto_word)sp;

    if (_this->type & Dds_Publisher)
    {
        CCortoDataPublisher *publisher = new CCortoDataPublisher(syncSubTopic+_this->topic);
        CCortoRequestSubscriber *subscriber = new CCortoRequestSubscriber(requestSubTopic+_this->topic);

        if (publisher->Initialize() == false)
        {
            corto_trace("Failed to initialize %s", "CCoreDataPublisher");
            delete publisher;
            delete subscriber;
        }
        else
        {
            if (subscriber->Initialize() == false)
            {
                corto_trace("Failed to initialize %s", "CCortoRequestSubscriber");
                delete subscriber;
                delete publisher;
            }
            else
            {
                CCortoRequestSubscriber::NewDataDelegate delegate(*sp,
                                             [&] (CCortoRequestSubscriber::Sample &sample){
                                                 dds_Connector_OnRequest(_this, sample);
                                             });
                subscriber->RegisterNewDataSubscriber(delegate);

                _this->request_subscriber = (corto_word)subscriber;
                _this->publisher = (corto_word)publisher;
            }
        }
    }
    if (_this->type & Dds_Subscriber)
    {
        CCortoDataSubscriber *subscriber = new CCortoDataSubscriber(syncSubTopic+_this->topic);
        CCortoRequestPublisher *publisher = new CCortoRequestPublisher(requestSubTopic+_this->topic);

        if (subscriber->Initialize() == false)
        {
            corto_trace("Failed to initialize %s", "CCoreDataSubscriber");
            delete subscriber;
            delete publisher;
        }
        else
        {
            if (publisher->Initialize() == false)
            {
                corto_trace("Failed to initialize %s", "CCortoRequestPublisher");
                delete subscriber;
                delete publisher;
            }
            else
            {
                CCortoDataSubscriber::NewDataDelegate delegate(*sp,
                                             [&] (CCortoDataSubscriber::Sample &sample){
                                                 dds_Connector_OnData(_this, sample);
                                             });
                subscriber->RegisterNewDataSubscriber(delegate);
                _this->subscriber = (corto_word)subscriber;
                _this->request_publisher = (corto_word)publisher;
            }
        }
    }
    /* << Insert implementation >> */
    return 0;
/* $end */
}

corto_void _dds_Connector_destruct(
    dds_Connector _this)
{
/* $begin(corto/dds/Connector/destruct) */
    corto_trace("Destruct");

    corto_trace("_this->ptr %o",_this->ptr);
    corto_trace("_this->publisher %o",_this->publisher );
    corto_trace("_this->subscriber %o",_this->subscriber );
    corto_trace("_this->request_publisher %o",_this->request_publisher );
    corto_trace("_this->request_subscriber %o",_this->request_subscriber );
    corto_trace("_this->data_handler %o",_this->data_handler );
    corto_trace("_this->request_handler %o",_this->request_handler);

    if (_this->data_handler != NULLWORD)
    {
        corto_trace("data_handler");
        core::InstanceHandle *handler = cortoCore_instanceHandle(_this->data_handler);
        cortoData_publisher(_this->publisher)->UnregisterInstance(*handler);
        corto_trace("delte data_handler");
        delete handler;
        _this->data_handler = NULLWORD;
    }
    if (_this->request_handler != NULLWORD)
    {

        core::InstanceHandle *handler = cortoCore_instanceHandle(_this->request_handler);
        corto_trace("request_handler %o", handler);
        cortoRequest_publisher(_this->request_publisher)->UnregisterInstance(*handler);
        corto_trace("delete request_handler %o", handler);

        delete handler;
        _this->request_handler = NULLWORD;
    }

    if (_this->subscriber != NULLWORD)
    {
        CCortoDataSubscriber *subscriber = cortoData_subscriber(_this->subscriber);
        subscriber->UnregisterNewDataSubscriber(*cortoSharedPtr_int(_this->ptr));
        delete subscriber;
        _this->subscriber = NULLWORD;
    }
    if (_this->request_subscriber != NULLWORD)
    {
        CCortoRequestSubscriber *subscriber = cortoRequest_subscriber(_this->request_subscriber);
        subscriber->UnregisterNewDataSubscriber(*cortoSharedPtr_int(_this->ptr));
        delete subscriber;
        _this->subscriber = NULLWORD;
    }

    if (_this->publisher != NULLWORD)
    {
        CCortoDataPublisher *publisher = cortoData_publisher(_this->publisher);
        delete publisher;
        _this->publisher = NULLWORD;
    }
    if (_this->request_publisher != NULLWORD)
    {
        CCortoRequestPublisher *publisher = cortoRequest_publisher(_this->request_publisher);
        delete publisher;
        _this->request_publisher = NULLWORD;
    }

    std::shared_ptr<int> *ptr = cortoSharedPtr_int(_this->ptr);
    delete ptr;
    _this->ptr = NULLWORD;
    corto_trace("End destructor");
/* $end */
}

corto_void _dds_Connector_onDeclare(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(corto/dds/Connector/onDeclare) */

    /* << Insert implementation >> */

/* $end */
}

corto_void _dds_Connector_onDelete(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(corto/dds/Connector/onDelete) */

    /* << Insert implementation >> */

/* $end */
}

corto_object _dds_Connector_onResume(
    dds_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object o)
{
/* $begin(corto/dds/Connector/onResume) */

    /* << Insert implementation >> */
    return NULL;
/* $end */
}

corto_void _dds_Connector_onUpdate(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(corto/dds/Connector/onUpdate) */

    /* << Insert implementation >> */

/* $end */
}

corto_void _dds_Connector_sendRequest(
    dds_Connector _this,
    corto_string name,
    corto_string type,
    corto_string value)
{
/* $begin(corto/dds/Connector/sendRequest) */
    if (_this->request_publisher == NULLWORD)
    {
        corto_trace("Error: try to send request without publisher");
        return;
    }
    Corto::Request request;//(name, type, value);
    request.name(name);
    request.type(type);
    request.value(value);

    CCortoRequestPublisher *publisher = cortoRequest_publisher(_this->request_publisher);

    core::InstanceHandle *handler = nullptr;
    if (_this->request_handler == NULLWORD)
    {
        handler = new core::InstanceHandle();
        *handler = publisher->RegisterInstance(request);
        _this->request_handler = (corto_word)handler;
    }
    else
    {
        handler = cortoCore_instanceHandle(_this->request_handler);
    }
    if (publisher->Write(request, *handler) == false)
    {
        corto_trace("Failed to write request.");
    }
/* $end */
}
