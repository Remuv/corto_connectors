/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/clmdb/clmdb.h>

/* $header() */
#include "lmdb_adapter.h"

#define TRACE() printf("%s:%i\n",__FILE__, __LINE__)

#define SAFE_STRING(str) std::string( str != nullptr ? str : "")

#define LMDB_Env(addr) (LMDB::MDBEnv*)(void*)(addr)

void clmdb_Connector_define (
    clmdb_Connector _this,
    corto_string parent,
    corto_string id,
    corto_string type,
    corto_string json
)
{
    LMDB::MDBEnv *m_env = LMDB_Env(_this->handle);
    if (m_env != nullptr)
    {
        std::string db = SAFE_STRING(parent);
        std::string name = SAFE_STRING(id);
        std::string _type = SAFE_STRING(type);
        std::string _json = SAFE_STRING(json);

        std::string data = _type + ":"+_json;
        m_env->DefineData(db, name, data);
    }
}

void clmdb_Connector_update (
    clmdb_Connector _this,
    corto_string parent,
    corto_string id,
    corto_string type,
    corto_string json
)
{
    LMDB::MDBEnv *m_env = LMDB_Env(_this->handle);
    if (m_env != nullptr)
    {
        std::string db = SAFE_STRING(parent);
        std::string name = SAFE_STRING(id);
        std::string _type = SAFE_STRING(type);
        std::string _json = SAFE_STRING(json);

        std::string data = _type + ":"+_json;
        m_env->UpdateData(db, name, data);
    }
}

void clmdb_Connector_delete(
    clmdb_Connector _this,
    corto_string parent,
    corto_string id
)
{
    LMDB::MDBEnv *m_env = LMDB_Env(_this->handle);

    if (m_env != nullptr)
    {
        std::string db = SAFE_STRING(parent);
        std::string name = SAFE_STRING(id);
        m_env->DeleteData(db, name);
    }
}

/* $end */

corto_int16 _clmdb_Connector_construct(
    clmdb_Connector _this)
{
/* $begin(recorto/clmdb/Connector/construct) */
    LMDB::MDBEnv *m_env = new LMDB::MDBEnv();
    m_env->Initialize(_this->path,
                     _this->flags,
                     _this->mode,
                     _this->map_size,
                     _this->update_rate);

    _this->handle = (corto_word)m_env;

    corto_mount_setContentType(_this, "text/json");

    corto_mount(_this)->kind = CORTO_SINK;
    return corto_mount_construct(_this);
/* $end */
}

corto_void _clmdb_Connector_destruct(
    clmdb_Connector _this)
{
/* $begin(recorto/clmdb/Connector/destruct) */
    LMDB::MDBEnv *m_env = LMDB_Env(_this->handle);
    if (m_env != nullptr)
    {
        m_env->Destroy();
        delete m_env;
    }
/* $end */
}

corto_void _clmdb_Connector_onNotify(
    clmdb_Connector _this,
    corto_eventMask event,
    corto_result *object)
{
/* $begin(recorto/clmdb/Connector/onNotify) */
    if (object->object != nullptr && !corto_checkAttr(object->object, CORTO_ATTR_PERSISTENT))
    {
        return;
    }

    if (event & CORTO_ON_DEFINE)
    {
        corto_string json = (corto_string)(void*)object->value;

        clmdb_Connector_define(_this,
                               object->parent,
                               object->id,
                               object->type,
                               json);
    }
    else if (event & CORTO_ON_UPDATE)
    {
        corto_string json = (corto_string)(void*)object->value;

        clmdb_Connector_update(_this,
                                object->parent,
                                object->id,
                                object->type,
                                json);
    }
    else if (event & CORTO_ON_DELETE)
    {
        clmdb_Connector_delete(_this,
                                object->parent,
                                object->id);
    }
/* $end */
}

/* $header(recorto/clmdb/Connector/onRequest) */

struct clmdb_iterData
{
    LMDB::Cursor cursor;
    std::string   parent;
    corto_result  result;
};

void *clmdb_iterNext(corto_iter *iter)
{
    clmdb_iterData *pData = (clmdb_iterData*)iter->udata;

    LMDB::Data data = pData->cursor.GetData();

    corto_id type;
    std::string strJson;
    corto_string json;

    char *ptr = (char*)data.m_data;
    char *ptr2 = &type[0];
    while (*ptr)
    {
        if (*ptr == ':')
        {
            *ptr2 = '\0';
            json = ++ptr;
            break;
        }
        *ptr2 = *ptr;
        ptr++;
        ptr2++;
    }

    strJson = std::string(json, data.m_size-(json-(char*)data.m_data));

    corto_setstr(&pData->result.id,(char*)data.m_key.c_str());
    corto_setstr(&pData->result.type, type);
    corto_setstr(&pData->result.parent, ".");
    pData->result.value = (corto_word)corto_strdup((char*)strJson.c_str());

    pData->cursor.Next();

    return &pData->result;
}

int clmdb_iterHasNext(corto_iter *iter)
{
    clmdb_iterData *pData =(clmdb_iterData*)iter->udata;

    int retVal = 0;

    if (pData->cursor.HasNext())
    {
        retVal = 1;
    }

    return retVal;
}

void clmdb_iterRelease(corto_iter *iter)
{
    typedef std::string CString;
    typedef LMDB::Cursor CCursor;
    clmdb_iterData *pData = (clmdb_iterData*)iter->udata;

    pData->parent.~CString();
    pData->cursor.~CCursor();

    corto_dealloc(pData);
}

/* $end */
corto_resultIter _clmdb_Connector_onRequest(
    clmdb_Connector _this,
    corto_request *request)
{
/* $begin(recorto/clmdb/Connector/onRequest) */
    corto_resultIter result;

    LMDB::MDBEnv *m_env = LMDB_Env(_this->handle);

    if (m_env != nullptr)
    {
        std::string parent = SAFE_STRING(request->parent);
        std::string expr = SAFE_STRING(request->expr);

        clmdb_iterData *data = (clmdb_iterData*)corto_calloc(sizeof(clmdb_iterData));

        new (&data->parent) std::string(parent);
        new (&data->cursor) LMDB::Cursor( m_env->GetCursor(parent, expr));

        data->result.id = NULL;
        data->result.name = NULL;
        data->result.parent = NULL;
        data->result.type = NULL;
        data->result.value = 0;
        data->result.leaf = false;

        result.udata = data;
        result.hasNext = clmdb_iterHasNext;
        result.next = clmdb_iterNext;
        result.release = clmdb_iterRelease;
    }

    return result;
/* $end */
}
