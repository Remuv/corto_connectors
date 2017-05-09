/* $CORTO_GENERATED
 *
 * ObjectConnector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/dds/dds.h>

/* $header() */
static corto_word gCounter = 0;

#include <sys/types.h>
#include "sync_adapter.h"
#include <sstream>

#define REPLACE_CHR(str, from, to) std::replace(str.begin(), str.end(), from, to)
#define SAFE_STRING(str) std::string(str != nullptr ? str : "")

#define StdSharedPtr_SyncAdapter(obj)((std::shared_ptr<CSyncAdapter>*)obj)
#define NULLWORD 0

corto_void dds_ObjectConnector_OnNewData(dds_ObjectConnector _this, CCortoDataSubscriber::Sample &sample)
{
    const Corto::Data &data = sample.data();

    std::string path = SAFE_STRING(corto_subscriber(_this)->parent);

    if (data.parent().size() > 0 && data.parent()[0] != '.')
    {
        path += "/" + data.parent();
    }

    path += "/" + data.id();

    char *type   = (char*)data.type().c_str();
    char *value  = (char*)data.value().c_str();

    corto_object prev = corto_setOwner(_this);

    corto_publish(CORTO_ON_UPDATE, (char*)path.c_str(), type, "text/json", value);

    corto_setOwner(prev);
}

corto_void dds_ObjectConnector_OnDisposeData(dds_ObjectConnector _this, CCortoDataSubscriber::Sample &sample)
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

    corto_object prev = corto_setOwner(_this);

    corto_publish(CORTO_ON_DELETE, (char*)path.c_str(), type, "text/json", value);

    corto_setOwner(prev);
}


/* $end */

corto_int16 _dds_ObjectConnector_construct(
    dds_ObjectConnector _this)
{
/* $begin(recorto/dds/ObjectConnector/construct) */
    std::shared_ptr<CSyncAdapter> *adapter = new std::shared_ptr<CSyncAdapter>();
    *adapter = std::shared_ptr<CSyncAdapter>(new CSyncAdapter(_this->topic));

    _this->dds_adapter = (corto_word)adapter;

    CSyncAdapter::DataNotifyCallback newDataCallback =
            [_this](CSyncAdapter::Sample &sample)
    {
        dds_ObjectConnector_OnNewData(_this, sample);
    };

    CSyncAdapter::DataNotifyCallback disposedDataCallback =
            [_this](CSyncAdapter::Sample &sample)
    {
        dds_ObjectConnector_OnDisposeData(_this, sample);
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

    return corto_mount_construct(_this);
/* $end */
}

corto_void _dds_ObjectConnector_destruct(
    dds_ObjectConnector _this)
{
/* $begin(recorto/dds/ObjectConnector/destruct) */
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

corto_void _dds_ObjectConnector_onNotify(
    dds_ObjectConnector _this,
    corto_eventMask event,
    corto_result *object)
{
/* $begin(recorto/dds/ObjectConnector/onNotify) */
    if(_this->dds_adapter == NULLWORD)
    {
        return;
    }
    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    if (event & CORTO_ON_DEFINE)
    {
        corto_object obj = object->object;
        std::string type = SAFE_STRING(object->type);
        std::string parent = SAFE_STRING(object->parent);
        std::string id = SAFE_STRING(object->id);
        std::string name = SAFE_STRING(object->name);
        if (name.empty())
        {
            name = id;
        }
        (*adapter)->CreateData(type, parent, id, name, obj);
    }
    else if (event & CORTO_ON_UPDATE)
    {
        corto_object obj = object->object;
        std::string type = SAFE_STRING(object->type);
        std::string parent = SAFE_STRING(object->parent);
        std::string id = SAFE_STRING(object->id);
        std::string name = SAFE_STRING(object->name);
        if (name.empty())
        {
            name = id;
        }
        (*adapter)->UpdateData(type, parent, id, name, obj);
    }
    else if (event & CORTO_ON_DELETE)
    {
        std::string parent = SAFE_STRING(object->parent);
        std::string id = SAFE_STRING(object->id);
        (*adapter)->DeleteData(parent, id);
    }
/* $end */
}

/* $header(recorto/dds/ObjectConnector/onRequest) */
struct dds_objIterData
{
    CSyncAdapter::SampleSeq::const_iterator iter;
    CSyncAdapter::SampleSeq samples;
    std::string parent;
    corto_result result;
};

void *dds_objIterNext(corto_iter *iter)
{
    dds_objIterData *pData = (dds_objIterData*)iter->udata;

    const Corto::Data &data =(*pData->iter).data();

    corto_setstr(&pData->result.id, (char*)data.id().c_str());
    corto_setstr(&pData->result.name, (char*)data.name().c_str());
    corto_setstr(&pData->result.type, (char*)data.type().c_str());
    corto_setstr(&pData->result.parent, ".");//(char*)pData->parent.c_str()); //(char*)data.parent().c_str());

    pData->iter++;
    return &pData->result;
}

int dds_objIterHasNext(corto_iter *iter)
{
    bool retVal = false;
    if (iter->udata != NULL)
    {
        dds_objIterData *pData = (dds_objIterData*)iter->udata;
        retVal = pData->iter != pData->samples.end();
    }
    return retVal;
}

void dds_objIterRelease(corto_iter *iter)
{
    if (iter->udata != NULL)
    {
        dds_objIterData *pData = (dds_objIterData*)iter->udata;
        pData->~dds_objIterData();
        corto_dealloc(pData);
    }
}
/* $end */
corto_resultIter _dds_ObjectConnector_onRequest(
    dds_ObjectConnector _this,
    corto_request *request)
{
/* $begin(recorto/dds/ObjectConnector/onRequest) */
    corto_resultIter result;

    result.udata = NULL;
    result.hasNext = dds_objIterHasNext;
    result.next = dds_objIterNext;
    result.release = dds_objIterRelease;

    if(_this->dds_adapter == NULLWORD)
    {
        return result;
    }

    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    std::string parent = SAFE_STRING(request->parent);
    std::string expr = SAFE_STRING(request->expr);

    dds_objIterData *data = (dds_objIterData*)corto_calloc(sizeof(dds_objIterData));

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

    bool updateNow = request->content;
    if ((*adapter)->Query(data->samples, parent, expr, updateNow) == false)
    {
        data->iter = data->samples.end();
    }
    else
    {
        data->iter = data->samples.begin();
    }
    return result;
/* $end */
}

corto_word _dds_ObjectConnector_onSubscribe(
    dds_ObjectConnector _this,
    corto_string parent,
    corto_string expr,
    corto_word ctx)
{
/* $begin(recorto/dds/ObjectConnector/onSubscribe) */
    if(_this->dds_adapter == NULLWORD)
    {
        return ctx;
    }
    std::string parentStr = SAFE_STRING(parent);
    std::string exprStr = SAFE_STRING(expr);

    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);
    (*adapter)->SubscribeData(parentStr, exprStr);

    return ++gCounter;
/* $end */
}

corto_word _dds_ObjectConnector_onUnsubscribe(
    dds_ObjectConnector _this,
    corto_string parent,
    corto_string expr,
    corto_word ctx)
{
/* $begin(recorto/dds/ObjectConnector/onUnsubscribe) */
    if(_this->dds_adapter == NULLWORD)
    {
        return ctx;
    }
    std::string parentStr = SAFE_STRING(parent);
    std::string exprStr = SAFE_STRING(expr);

    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);
    (*adapter)->UnsubscribeData(parentStr, exprStr);

    return ++gCounter;
/* $end */
}
