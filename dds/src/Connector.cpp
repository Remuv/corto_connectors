/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/dds/dds.h>

/* $header() */
#include "sync_adapter.h"
#include <sstream>

#define StdSharedPtr_SyncAdapter(obj)((std::shared_ptr<CSyncAdapter>*)obj)
#define NULLWORD 0

corto_void dds_Connector_SendData(dds_Connector _this, corto_object obj)
{
    if(_this->dds_adapter == NULLWORD)
    {
        return;
    }
    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    corto_id name;
    corto_id type;
    corto_path(name, corto_mount(_this)->mount, obj, ".");
    corto_fullpath(type, corto_typeof(obj));
    corto_string cstr = corto_str(obj, 0);
    std::string value;
    if(cstr[0] == '{')
    {
        value = std::string(cstr);
    }
    else
    {
        value = "{"+std::string(cstr)+"}";
    }
    (*adapter)->SendData(name, type, value);
    corto_dealloc(cstr);
}

corto_void dds_Connector_OnRequest(dds_Connector _this, CCortoRequestSubscriber::Sample &sample)
{
    Corto::Request request = sample.data();

    if (strcmp(request.type().c_str(), "UPDATE") == 0)
    {
        corto_object obj = corto_resolve(corto_mount(_this)->mount, (char*)request.name().c_str());
        if (obj != nullptr)
        {
            const char *name = request.name().c_str();
            const char *value = request.value().c_str();

            if (corto_updateBegin(obj) == 0)
            {
                if (corto_fromStr(&obj, (char*)value) != 0)
                {
                    corto_error("Failed to deserialize for %s: %s (%s)", (char*)name, corto_lasterr(), (char*)value);
                }
                corto_updateEnd(obj);
            }
            corto_release(obj);
        }
    }
    else if (strcmp(request.type().c_str(), "REQUEST") == 0)
    {
        if (strcmp(request.name().c_str(), "*") == 0)
        {
            corto_object obj = NULL;
            corto_id name;
            corto_id type;
            corto_id path;
            corto_fullpath(path, corto_mount(_this)->mount);
            corto_iter iter = corto_select(path, "*").iter(NULL);

            std::string value;
            corto_resultIterForeach(iter, e) {
                obj = corto_resolve(corto_mount(_this)->mount, e.name);
                if (obj != NULL)
                {
                    if (value.empty() == false)
                    {
                        value += "\n";
                    }
                    corto_path(name, corto_mount(_this)->mount, obj, ".");
                    corto_fullpath(type, corto_typeof(obj));
                    corto_string cstr = corto_str(obj, 0);
                    if(cstr[0] == '{')
                    {
                        value += std::string(type)+","+std::string(name)+","+std::string(cstr);
                    }
                    else
                    {
                        value += std::string(type)+","+std::string(name)+",{"+std::string(cstr)+"}";
                    }
                    corto_dealloc(cstr);
                    corto_release(obj);
                }
            }
            std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);
            (*adapter)->SendData("", "", value);
        }
        else
        {
            corto_object obj = corto_resolve(corto_mount(_this)->mount, (char*)request.name().c_str());

            if (obj != nullptr)
            {
                dds_Connector_SendData(_this, obj);
                corto_release(obj);
            }
        }
    }
}

corto_void dds_Connector_SetData(dds_Connector _this, corto_string type, corto_string name, corto_string value)
{
    corto_object obj = corto_resolve(corto_mount(_this)->mount, name);
    if (obj != nullptr)
    {
        corto_string cstr = corto_str(obj, 0);
        std::string valStr;
        if(cstr[0] == '{')
        {
            valStr = std::string(cstr);
        }
        else
        {
            valStr = "{"+std::string(cstr)+"}";
        }
        corto_dealloc(cstr);

        if (strcmp(value, valStr.c_str()) != 0)
        {
            if (corto_updateBegin(obj) == 0)
            {
                if (corto_fromStr(&obj, value) != 0)
                {
                    corto_error("Failed to deserialize for %s,%s: %s (%s)", type, name, corto_lasterr(), value);
                }
                corto_updateEnd(obj);
            }
        }
        corto_release(obj);
    }
    else
    {
        corto_object typeo = corto_resolve(NULL,type);
        if (typeo == NULL)
        {
            corto_error("Type %s not found", type);
            return;
        }
        obj = corto_declareChild(corto_mount(_this)->mount, name, typeo);
        corto_release(typeo);
        if (obj == NULL)
        {
            corto_error("Failed to create object %s", name);
            return;
        }
        if (corto_fromStr(&obj, value) != 0)
        {
            corto_error("Failed to deserialize for %s,%s: %s (%s)", type, name, corto_lasterr(), value);
            return;
        }
        if (corto_define(obj) != 0)
        {
            corto_error("Failed to define %s", corto_idof(obj));
        }
    }
}

corto_void dds_Connector_OnData(dds_Connector _this, CCortoDataSubscriber::Sample &sample)
{
    Corto::Data data = sample.data();

    if (data.type().empty())
    {
        std::stringstream iss(data.value());
        std::string line;

        while(std::getline(iss, line, '\n'))
        {
            if(line.empty() == false)
            {
                size_t n;
                size_t v;
                if ( ((n = line.find(',')) != std::string::npos) &&
                     ((v = line.find(',', n+1)) != std::string::npos) )
                {
                    std::string type = line.substr(0,n);
                    std::string name = line.substr(n+1,v - (n+1));
                    std::string value = line.substr(v+1);
                    dds_Connector_SetData(_this, (char*)type.c_str(), (char*)name.c_str(), (char*)value.c_str());
                }
            }
        }
    }
    else
    {
        char *name = (char*)data.name().c_str();
        char *type = (char*)data.type().c_str();
        char *value = (char*)data.value().c_str();
        dds_Connector_SetData(_this, type, name, value);
    }
}
/* $end */

corto_int16 _dds_Connector_construct(
    dds_Connector _this)
{
/* $begin(corto/dds/Connector/construct) */
    std::shared_ptr<CSyncAdapter> *adapter = new std::shared_ptr<CSyncAdapter>();
    *adapter = std::shared_ptr<CSyncAdapter>(new CSyncAdapter(_this->topic));

    _this->dds_adapter = (corto_word)adapter;

    if (_this->type & Dds_Publisher)
    {
        CCortoRequestSubscriber::DataNotifyCallback callback =
                [_this](CCortoRequestSubscriber::Sample &sample)
                {
                    dds_Connector_OnRequest(_this, sample);
                };

        if ((*adapter)->SetUpDataPublisher(callback) == false)
        {
            (*adapter)->Close();
            adapter->reset();
            delete adapter;
            _this->dds_adapter = NULLWORD;
            corto_seterr("Fail to Initialize Data Publisher");
            return -1;
        }
    }
    if (_this->type & Dds_Subscriber)
    {
        CCortoDataSubscriber::DataNotifyCallback callback =
                [_this](CCortoDataSubscriber::Sample &sample)
                {
                    dds_Connector_OnData(_this, sample);
                };
        if ((*adapter)->SetUpDataSubscriber(callback) == false)
        {
            (*adapter)->Close();
            adapter->reset();
            delete adapter;
            _this->dds_adapter = NULLWORD;
            corto_seterr("Fail to Initialize Data Subscriber");
            return -1;
        }
    }
    corto_setstr(&corto_mount(_this)->type, "/noType");

    corto_mount(_this)->mask = CORTO_ON_SCOPE | CORTO_ON_TREE;
    corto_mount(_this)->kind = CORTO_SINK;
    return corto_mount_construct(_this);
/* $end */
}

corto_void _dds_Connector_destruct(
    dds_Connector _this)
{
/* $begin(corto/dds/Connector/destruct) */
    if(_this->dds_adapter == NULLWORD)
    {
        return;
    }
    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    (*adapter)->Close();
    adapter->reset();
    delete adapter;
    _this->dds_adapter = NULLWORD;
/* $end */
}

corto_void _dds_Connector_onDeclare(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(corto/dds/Connector/onDeclare) */

/* $end */
}

corto_void _dds_Connector_onDelete(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(corto/dds/Connector/onDelete) */

/* $end */
}

corto_resultIter _dds_Connector_onRequest(
    dds_Connector _this,
    corto_request *request)
{
/* $begin(corto/dds/Connector/onRequest) */
    /* << Insert implementation >> */
    return corto_mount_onRequest_v(_this, request);
/* $end */
}

corto_object _dds_Connector_onResume(
    dds_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object o)
{
/* $begin(corto/dds/Connector/onResume) */
    return o;
/* $end */
}

corto_void _dds_Connector_onUpdate(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(corto/dds/Connector/onUpdate) */
    dds_Connector_SendData(_this, observable);
/* $end */
}

corto_void _dds_Connector_sendRequest(
    dds_Connector _this,
    corto_string name,
    corto_string type,
    corto_string value)
{
/* $begin(corto/dds/Connector/sendRequest) */
    if(_this->dds_adapter == NULLWORD)
    {
        return;
    }
    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    (*adapter)->SendRequest(name, type, value);
/* $end */
}
