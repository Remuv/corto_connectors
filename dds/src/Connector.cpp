/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/dds/dds.h>

/* $header() */

#include <sys/types.h>

#include "sync_adapter.h"
#include <sstream>

#define SAFE_STRING(str) std::string(str != nullptr ? str : "")

#define StdSharedPtr_SyncAdapter(obj)((std::shared_ptr<CSyncAdapter>*)obj)
#define NULLWORD 0
#define NEWLINEDELIM "EOL\n"

corto_object corto_createChildrenRecursive(corto_object parent, corto_string path, corto_object type)
{
    corto_object retObj = parent;
    if (path != nullptr && *path != '\0')
    {
        if (type == nullptr)
        {
            type = corto_void_o;
        }
        corto_id name;
        char *ptr1 = path, *ptr2 = &name[0];
        while (*ptr1 != '\0')
        {
            if(*ptr1 == '.' || *ptr1 == '/')
            {
                ptr1++;
                break;
            }
            *ptr2 = *ptr1;
            ptr1++;
            ptr2++;
        }
        *ptr2 = '\0';

        corto_object obj = corto_declareChild(parent, name, type);
        if (obj != nullptr)
        {
            corto_define(obj);
            retObj = corto_createChildrenRecursive(obj, ptr1, type);
        }
    }
    return retObj;
}

corto_void dds_Connector_SendData(dds_Connector _this, corto_object obj)
{
    if(_this->dds_adapter == NULLWORD)
    {
        return;
    }
    corto_object parent_o = corto_parentof(obj);

    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    corto_id type;
    corto_id parent;
    corto_string name;

    corto_fullpath(type, corto_typeof(obj));
    name = corto_idof(obj);
    corto_path(parent, corto_mount(_this)->mount, parent_o, "/");
    corto_cleanpath(parent, parent);
    corto_string cstr = corto_str(obj, 0);
    std::string value;
    if(cstr == nullptr)
    {
        value = std::string("{}");
    }
    else
    {
        if(cstr[0] == '{')
        {
            value = std::string(cstr);
        }
        else
        {
            value = "{"+std::string(cstr)+"}";
        }
        corto_dealloc(cstr);
    }
    if ((*adapter)->SendData(type, parent, name, value) == false)
    {
        std::string rin = SAFE_STRING(parent) + "/" + SAFE_STRING(name);
        (*adapter)->SendRequest(rin, "UPDATE", value);
    }
}

corto_void dds_Connector_OnRequest(dds_Connector _this, CCortoRequestSubscriber::Sample &sample)
{
    Corto::Request request = sample.data();

    if (strcmp(request.type().c_str(), "UPDATE") == 0)
    {
        corto_object obj = corto_lookup(corto_mount(_this)->mount, (char*)request.name().c_str());
        if (obj != nullptr)
        {
            const char *name = request.name().c_str();
            const char *value = request.value().c_str();
            if (corto_updateBegin(obj) == 0)
            {
                if (corto_fromStr(&obj, (char*)value) != 0)
                {
                    corto_error("[OnRequest] Failed to deserialize for %s: %s (%s)", (char*)name, corto_lasterr(), (char*)value);
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
            std::string value;

            corto_object obj = NULL;
            corto_id path;

            corto_fullpath(path, corto_mount(_this)->mount);
            corto_resultIter iter;

            corto_select(path, "//*").iter(&iter);

            corto_resultIterForeach(iter, e) {
                sprintf(path, "%s/%s", e.parent, e.name);
                corto_cleanpath(path, path);

                obj = corto_lookup(corto_mount(_this)->mount, path);
                if (obj != NULL)
                {
                    corto_string cstr = corto_str(obj, 0);
                    if (cstr == nullptr)
                    {
                        value += std::string(e.type)+","+std::string(e.parent)+","+std::string(e.name)+",{}";
                    }
                    else
                    {
                        if (cstr[0] == '{')
                        {
                            value += std::string(e.type)+","+std::string(e.parent)+","+std::string(e.name)+","+std::string(cstr);
                        }
                        else
                        {
                            value += std::string(e.type)+","+std::string(e.parent)+","+std::string(e.name)+",{"+std::string(cstr)+"}";
                        }
                        corto_dealloc(cstr);
                    }
                    value += NEWLINEDELIM;
                    corto_release(obj);
                }
            }
            std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);
            (*adapter)->SendData("","", "", value);
        }
        else
        {
            corto_object obj = corto_lookup(corto_mount(_this)->mount, (char*)request.name().c_str());

            if (obj != nullptr)
            {
                dds_Connector_SendData(_this, obj);
                corto_release(obj);
            }
        }
    }
}

corto_void dds_Connector_SetData(dds_Connector _this, corto_string type, corto_string parent, corto_string name, corto_string value)
{
    corto_object prev = corto_setOwner(_this);
    corto_object parent_o = corto_lookup(corto_mount(_this)->mount, parent);
    if (parent_o == nullptr)
    {
        if (parent == nullptr || *parent == '\0' || *parent == '.')
        {
            parent_o = corto_mount(_this)->mount;
        }
        else
        {
            corto_object typeo = corto_resolve(NULL,type);
            parent_o = corto_createChildrenRecursive(corto_mount(_this)->mount, parent, typeo);
            corto_release(typeo);
        }
    }
    corto_object obj = corto_lookup(parent_o, name);
    if (obj != nullptr)
    {
        corto_string cstr = corto_str(obj, 0);
        std::string valStr;

        if (cstr == nullptr)
        {
            valStr = "{}";
        }
        else
        {
            if (cstr[0] == '{')
            {
                valStr = std::string(cstr);
            }
            else
            {
                valStr = "{"+std::string(cstr)+"}";
            }
            corto_dealloc(cstr);
        }

        if (strcmp(value, valStr.c_str()) != 0)
        {
            if (corto_updateBegin(obj) == 0)
            {
                if (corto_fromStr(&obj, value) != 0)
                {
                    corto_error("[SetData] Failed to deserialize for %s,%s: (%s) error: %s",
                        type,
                        name,
                        value,
                        corto_lasterr());
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
        obj = corto_declareChild(parent_o, name, typeo);
        corto_release(typeo);
        if (obj == NULL)
        {
            corto_error("Failed to create object %s", name);
        }
        else if (corto_fromStr(&obj, value) != 0)
        {
            corto_error("[SetData, new obj] Failed to deserialize for %s,%s: %s (%s)", type, name, corto_lasterr(), value);
        }
        else if (corto_define(obj) != 0)
        {
            corto_error("Failed to define %s", corto_idof(obj));
        }
    }
    corto_setOwner(prev);
}

std::vector<std::string> StrSplit(std::string str, std::string delim)
{
    std::vector<std::string> lines;
    int start = 0;
    size_t end = 0;

    while ((end = str.find(delim, start)) != std::string::npos)
    {
        int size = end - start;
        lines.push_back(str.substr(start, size));
        start = end+delim.size();
    }
    return lines;
}

corto_void dds_Connector_OnData(dds_Connector _this, CCortoDataSubscriber::Sample &sample)
{
    Corto::Data data = sample.data();
    if (data.type().empty())
    {
        std::vector<std::string> lines = StrSplit(data.value(), NEWLINEDELIM);

        for (size_t i = 0; i < lines.size(); i++)
        {
            std::string line = lines[i];
            if(line.empty() == false)
            {
                size_t p;
                size_t n;
                size_t v;
                if (((p = line.find(',')) != std::string::npos) &&     //find index of parent
                     ((n = line.find(',', p+1)) != std::string::npos) &&//find index of name
                     ((v = line.find(',', n+1)) != std::string::npos))  //find index of value
                {
                    std::string type   = line.substr(0,p);
                    std::string parent = line.substr(p+1, n-(p+1));
                    std::string name   = line.substr(n+1, v-(n+1));
                    std::string value  = line.substr(v+1);
                    dds_Connector_SetData(_this, (char*)type.c_str(), (char*)parent.c_str(), (char*)name.c_str(), (char*)value.c_str());
                }
            }
        }
    }
    else
    {
        char *type   = (char*)data.type().c_str();
        char *parent = (char*)data.parent().c_str();
        char *name   = (char*)data.name().c_str();
        char *value  = (char*)data.value().c_str();

        dds_Connector_SetData(_this, type, parent, name, value);
    }
}
/* $end */

corto_int16 _dds_Connector_construct(
    dds_Connector _this)
{
/* $begin(recorto/dds/Connector/construct) */

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
    //corto_setstr(&corto_mount(_this)->type, "/noType");

    corto_observer(_this)->mask = CORTO_ON_SCOPE | CORTO_ON_TREE;
    corto_mount(_this)->kind = CORTO_SINK;
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

corto_void _dds_Connector_onDeclare(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(recorto/dds/Connector/onDeclare) */
    dds_Connector_SendData(_this, observable);
/* $end */
}

corto_void _dds_Connector_onDelete(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(recorto/dds/Connector/onDelete) */

/* $end */
}

corto_object _dds_Connector_onResume(
    dds_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object o)
{
/* $begin(recorto/dds/Connector/onResume) */
    return o;
/* $end */
}

corto_void _dds_Connector_onUpdate(
    dds_Connector _this,
    corto_object observable)
{
/* $begin(recorto/dds/Connector/onUpdate) */
    //corto_id name;
    //corto_fullpath(name, observable);
    //printf("OnUpdate %s\n", name);
    dds_Connector_SendData(_this, observable);
/* $end */
}

corto_void _dds_Connector_sendRequest(
    dds_Connector _this,
    corto_string name,
    corto_string type,
    corto_string value)
{
/* $begin(recorto/dds/Connector/sendRequest) */
    if(_this->dds_adapter == NULLWORD)
    {
        return;
    }
    std::shared_ptr<CSyncAdapter> *adapter = StdSharedPtr_SyncAdapter(_this->dds_adapter);

    (*adapter)->SendRequest(name, type, value);
/* $end */
}
