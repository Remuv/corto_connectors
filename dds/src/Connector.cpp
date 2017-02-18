/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/dds/dds.h>

/* $header() */
static corto_word gCounter = 0;

// #define TRACE(fmt, args...) printf("%s:%i, %s: " fmt "\n", __FILE__, __LINE__, __func__, args)
#define TRACE(fnt, args...)

#include <sys/types.h>

#include "sync_adapter.h"
#include <sstream>


#define REPLACE_CHR(str, from, to) std::replace(str.begin(), str.end(), from, to)
#define SAFE_STRING(str) std::string(str != nullptr ? str : "")

#define StdSharedPtr_SyncAdapter(obj)((std::shared_ptr<CSyncAdapter>*)obj)
#define NULLWORD 0

corto_void dds_Connector_OnNewData(dds_Connector _this, CCortoDataSubscriber::Sample &sample)
{
    const Corto::Data &data = sample.data();

    std::string path = SAFE_STRING(corto_subscriber(_this)->parent);

    if (data.parent().size() > 0 && data.parent()[0] != '.')
    {
        path += "/" + data.parent();
    }

    path += "/" + data.id();
    // REPLACE_CHR(path, '.', '/');
    char *type   = (char*)data.type().c_str();
    char *value  = (char*)data.value().c_str();

    // TRACE("ON_DATA[%s]: path:%s, t:%s, v:%s", corto_idof(_this), (char*)path.c_str(), type, value);

    corto_object prev = corto_setOwner(_this);

    corto_publish(CORTO_ON_UPDATE, (char*)path.c_str(), type, "text/json", value);

    corto_setOwner(prev);
}

corto_void dds_Connector_OnDisposeData(dds_Connector _this, CCortoDataSubscriber::Sample &sample)
{
    const Corto::Data &data = sample.data();

    std::string path = SAFE_STRING(corto_subscriber(_this)->parent);

    if (data.parent().size() > 0 && data.parent()[0] != '.')
    {
        path += "/" + data.parent();
    }

    path += "/" + data.id();

    REPLACE_CHR(path, '.', '/');

    char *type   = (char*)data.type().c_str();
    char *value  = (char*)data.value().c_str();

    // TRACE("ON_DISPOSE[%s]: path:%s, t:%s, v:%s", corto_idof(_this), (char*)path.c_str(), type, value);

    corto_object prev = corto_setOwner(_this);

    corto_publish(CORTO_ON_DELETE, (char*)path.c_str(), type, "text/json", value);

    corto_setOwner(prev);
}
/* $end */

corto_int16 _dds_Connector_construct(
    dds_Connector _this)
{
/* $begin(recorto/dds/Connector/construct) */

    std::shared_ptr<CSyncAdapter> *adapter = new std::shared_ptr<CSyncAdapter>();
    *adapter = std::shared_ptr<CSyncAdapter>(new CSyncAdapter(_this->topic));

    _this->dds_adapter = (corto_word)adapter;

    CSyncAdapter::DataNotifyCallback newDataCallback =
            [_this](CSyncAdapter::Sample &sample)
    {
        dds_Connector_OnNewData(_this, sample);
    };

    CSyncAdapter::DataNotifyCallback disposedDataCallback =
            [_this](CSyncAdapter::Sample &sample)
    {
        dds_Connector_OnDisposeData(_this, sample);
    };

    if ((*adapter)->Initialize(newDataCallback, disposedDataCallback, _this->update_rate) == false)
    {
        (*adapter)->Close();
        adapter->reset();
        delete adapter;
        _this->dds_adapter = NULLWORD;
        corto_seterr("Failed to Initialize DDS Interface");
        return -1;
    }

    corto_observer(_this)->mask = CORTO_ON_TREE;
    corto_mount(_this)->kind = CORTO_SINK;
    corto_mount_setContentType(_this, "text/json");

    return corto_mount_construct(_this);
/* $end */
}

corto_void _dds_Connector_destruct(
    dds_Connector _this)
{
/* $begin(recorto/dds/Connector/destruct) */
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

corto_void _dds_Connector_onNotify(
    dds_Connector _this,
    corto_eventMask event,
    corto_result *object)
{
/* $begin(recorto/dds/Connector/onNotify) */
    if(_this->dds_adapter == NULLWORD)
    {
        return;
    }
    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    if (event & CORTO_ON_DEFINE)
    {
        corto_string json = (corto_string)(void*)object->value;
        std::string type = SAFE_STRING(object->type);
        std::string parent = SAFE_STRING(object->parent);
        std::string id = SAFE_STRING(object->id);
        std::string name = SAFE_STRING(object->name);
        std::string value = SAFE_STRING(json);
        if (name.empty())
        {
            name = id;
        }
        // TRACE("ON_DEFINE[%s]: p:%s, n:%s, t:%s, v:%s",corto_idof(_this), object->parent, object->id, object->type, json);
        (*adapter)->CreateData(type, parent, id, name, value);
    }
    else if (event & CORTO_ON_UPDATE)
    {
        corto_string json = (corto_string)(void*)object->value;
        std::string type = SAFE_STRING(object->type);
        std::string parent = SAFE_STRING(object->parent);
        std::string id = SAFE_STRING(object->id);
        std::string name = SAFE_STRING(object->name);
        std::string value = SAFE_STRING(json);
        if (name.empty())
        {
            name = id;
        }
        // TRACE("ON_UPDATE[%s]: p:%s, n:%s, t:%s, v:%s",corto_idof(_this), object->parent, object->id, object->type, json);
        (*adapter)->UpdateData(type, parent, id, name, value);
    }
    else if (event & CORTO_ON_DELETE)
    {
        // TRACE("ON_DELETE[%s]: p:%s, n:%s",corto_idof(_this), object->parent, object->id);
        std::string parent = SAFE_STRING(object->parent);
        std::string id = SAFE_STRING(object->id);
        (*adapter)->DeleteData(parent, id);
    }
/* $end */
}

/* $header(recorto/dds/Connector/onRequest) */
struct dds_iterData
{
    CSyncAdapter::SampleSeq::const_iterator iter;
    CSyncAdapter::SampleSeq samples;
    std::string parent;
    corto_result result;
};

void *dds_iterNext(corto_iter *iter)
{
    dds_iterData *pData = (dds_iterData*)iter->udata;

    const Corto::Data &data =(*pData->iter).data();

    corto_setstr(&pData->result.id, (char*)data.id().c_str());
    corto_setstr(&pData->result.name, (char*)data.name().c_str());
    corto_setstr(&pData->result.type, (char*)data.type().c_str());
    corto_setstr(&pData->result.parent, ".");//(char*)pData->parent.c_str()); //(char*)data.parent().c_str());
    pData->result.value = (corto_word)corto_strdup(data.value().c_str());

    pData->iter++;
    // TRACE("%s, %s, %s, %s", pData->result.type, pData->result.parent, pData->result.id, (char*)pData->result.value);
    return &pData->result;
}

int dds_iterHasNext(corto_iter *iter)
{
    bool retVal = false;
    if (iter->udata != NULL)
    {
        dds_iterData *pData = (dds_iterData*)iter->udata;
        retVal = pData->iter != pData->samples.end();
    }
    return retVal;
}

void dds_iterRelease(corto_iter *iter)
{
    if (iter->udata != NULL)
    {
        dds_iterData *pData = (dds_iterData*)iter->udata;
        pData->~dds_iterData();
        corto_dealloc(pData);
    }
}
/* $end */
corto_resultIter _dds_Connector_onRequest(
    dds_Connector _this,
    corto_request *request)
{
/* $begin(recorto/dds/Connector/onRequest) */

    corto_resultIter result;

    result.udata = NULL;
    result.hasNext = dds_iterHasNext;
    result.next = dds_iterNext;
    result.release = dds_iterRelease;

    if(_this->dds_adapter == NULLWORD)
    {
        return result;
    }

    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    std::string parent = SAFE_STRING(request->parent);
    std::string expr = SAFE_STRING(request->expr);

    dds_iterData *data = (dds_iterData*)corto_calloc(sizeof(dds_iterData));

    new (&data->samples) CSyncAdapter::SampleSeq();
    new (&data->iter) CSyncAdapter::SampleSeq::const_iterator();
    new (&data->parent) std::string(parent);

    data->result.id = NULL;
    data->result.name = NULL;
    data->result.parent = NULL;
    data->result.type = NULL;
    data->result.value = 0;
    data->result.leaf = false;

    result.udata = data;

    // TRACE("onRequest[%s] %s, %s", corto_idof(_this), parent.c_str(), expr.c_str());
    bool updateNow = request->content;
    if ((*adapter)->Query(data->samples, parent, expr, updateNow) == false)
    {
        data->iter = data->samples.end();
    }
    else
    {
        // TRACE("Count %i", data->samples.length());
        data->iter = data->samples.begin();
    }
    return result;
/* $end */
}

corto_word _dds_Connector_onSubscribe(
    dds_Connector _this,
    corto_string parent,
    corto_string expr,
    corto_word ctx)
{
/* $begin(recorto/dds/Connector/onSubscribe) */
    if(_this->dds_adapter == NULLWORD)
    {
        return ctx;
    }
    std::string parentStr = SAFE_STRING(parent);
    std::string exprStr = SAFE_STRING(expr);

    TRACE("onSubscribe[%s] %s, %s", corto_idof(_this), parentStr.c_str(), exprStr.c_str());

    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);
    (*adapter)->SubscribeData(parentStr, exprStr);

    return ++gCounter;
/* $end */
}

corto_word _dds_Connector_onUnsubscribe(
    dds_Connector _this,
    corto_string parent,
    corto_string expr,
    corto_word ctx)
{
/* $begin(recorto/dds/Connector/onUnsubscribe) */
    if(_this->dds_adapter == NULLWORD)
    {
        return ctx;
    }
    std::string parentStr = SAFE_STRING(parent);
    std::string exprStr = SAFE_STRING(expr);

    TRACE("onUnsubscribe[%s] %s, %s", corto_idof(_this), parentStr.c_str(), exprStr.c_str());

    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);
    (*adapter)->UnsubscribeData(parentStr, exprStr);

    return ++gCounter;
/* $end */
}
