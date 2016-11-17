/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/mongo/mongo.h>

/* $header() */
#include "mongo_util.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/builder/core.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>

#include <mongocxx/stdx.hpp>
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using bsoncxx::document::element;

#define SAFE_STRING(str) std::string( str != nullptr ? str : "")

extern corto_uint8 MONGOPOOL_HANDLE;

void mongo_Connector_update(
    mongo_Connector _this,
    corto_string parent,
    corto_string id,
    corto_string type,
    corto_string json
)
{
    std::string database = SAFE_STRING(_this->dbname);
    std::string collection = SAFE_STRING(parent);
    std::string name = SAFE_STRING(id);
    std::string _type = SAFE_STRING(type);
    std::string _data = SAFE_STRING(json);

    try
    {
        CMongoPool *pPool = (CMongoPool*)corto_olsGet(_this, MONGOPOOL_HANDLE);

        std::string parent_s = collection;

        if (collection == ".") {
            collection = "/";
        }

        MongoClientPtr pClient = pPool->GetClient();
        mongocxx::collection coll = (*pClient)[database][collection];

        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "id" << name;

        auto cursor = coll.find_one(filter_builder.view());
        if (!cursor)
        {
            bsoncxx::builder::stream::document update_builder;
            update_builder  << "id" << name
                            << "value" << _data
                            << "meta" << open_document
                              << "type" << _type
                              << "parent" << parent_s
                            << close_document;
            coll.insert_one(update_builder.view());
        }
        else
        {
            bsoncxx::builder::stream::document update_builder;
            update_builder << "$set" << open_document
                            << "value" << _data
                            << "meta" << open_document
                              << "type" << _type
                              << "parent" << parent_s
                            << close_document
                           << close_document;
            coll.update_one(filter_builder.view(), update_builder.view());
        }
    }
    catch(std::exception &e)
    {
        corto_error("%s", e.what());
    }
}

void mongo_Connector_delete(
    mongo_Connector _this,
    corto_string parent,
    corto_string id
)
{
    std::string database = SAFE_STRING(_this->dbname);
    std::string collection = SAFE_STRING(parent);
    std::string name = SAFE_STRING(id);
    try
    {
        CMongoPool *pPool = (CMongoPool*)corto_olsGet(_this, MONGOPOOL_HANDLE);

        if (collection == ".") {
            collection = "/";
        }

        MongoClientPtr pClient = pPool->GetClient();
        mongocxx::collection coll = (*pClient)[database][collection];

        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "id" << name;

        coll.delete_many(filter_builder.view());
    }
    catch(std::exception &e)
    {
        corto_error("%s", e.what());
    }
}

/* $end */

corto_int16 _mongo_Connector_construct(
    mongo_Connector _this)
{
/* $begin(recorto/mongo/Connector/construct) */
    if (!corto_checkAttr(_this, CORTO_ATTR_SCOPED))
    {
        corto_seterr("mongo/Connector objects must be SCOPED");
        return -1;
    }

    CMongoPool *pPool = new CMongoPool();

    pPool->Initialize(SAFE_STRING(_this->user),
                        SAFE_STRING(_this->password),
                        SAFE_STRING(_this->hostaddr),
                        std::to_string(_this->port));
    corto_olsSet(_this, MONGOPOOL_HANDLE, pPool);

    corto_mount_setContentType(_this, "text/json");
    corto_mount(_this)->kind = CORTO_SINK;

    return corto_mount_construct(_this);
/* $end */
}

corto_void _mongo_Connector_onNotify(
    mongo_Connector _this,
    corto_eventMask event,
    corto_result *object)
{
/* $begin(recorto/mongo/Connector/onNotify) */
    if (event & CORTO_ON_DEFINE)
    {
        corto_string json = (corto_string)(void*)object->value;

        mongo_Connector_update(_this,
                                object->parent,
                                object->id,
                                object->type,
                                json);
    }
    else if (event & CORTO_ON_UPDATE)
    {
        corto_string json = (corto_string)(void*)object->value;

        mongo_Connector_update(_this,
                                object->parent,
                                object->id,
                                object->type,
                                json);
    }
    else if (event & CORTO_ON_DELETE)
    {
        mongo_Connector_delete(_this,
                                object->parent,
                                object->id);
    }

/* $end */
}

/* $header(recorto/mongo/Connector/onRequest) */
bool check_document(bsoncxx::document::view &doc)
{

    element name = doc["id"];
    if (!name || name.type() != bsoncxx::type::k_utf8)
    {
        return false;
    }

    element value = doc["value"];
    if (!value || value.type() != bsoncxx::type::k_utf8)
    {
        return false;
    }

    element meta = doc["meta"];
    if (!meta || meta.type() != bsoncxx::type::k_document)
    {
        return false;
    }

    bsoncxx::document::view view = meta.get_document().view();
    element type = view["type"];
    if (!type || type.type() != bsoncxx::type::k_utf8)
    {
        return false;
    }

    element parent = view["parent"];
    if (!parent || parent.type() != bsoncxx::type::k_utf8)
    {
        return false;
    }

    return true;
}


struct mongodb_iterData
{
    MongoClientPtr client;
    std::string parent;
    mongocxx::cursor res;
    mongocxx::cursor::iterator iter;
    corto_result result;
};

void *mongodb_iterNext(corto_iter *iter)
{
    mongodb_iterData *pData = (mongodb_iterData*)iter->udata;
    bsoncxx::document::view data = *pData->iter;

    element name = data["id"];
    corto_setstr(&pData->result.id, (char*)name.get_utf8().value.to_string().c_str());

    element value = data["value"];
    pData->result.value = (corto_word)corto_strdup((char*)value.get_utf8().value.to_string().c_str());

    element meta = data["meta"];

    bsoncxx::document::view view = meta.get_document().view();

    element type = view["type"];
    corto_setstr(&pData->result.type, (char*)type.get_utf8().value.to_string().c_str());

    element parent = view["parent"];
    corto_setstr(&pData->result.parent, (char*)parent.get_utf8().value.to_string().c_str());

    pData->result.name = NULL;
    pData->iter++;

    return &pData->result;
}

int mongodb_iterHasNext(corto_iter *iter)
{
    mongodb_iterData *pData = (mongodb_iterData*)iter->udata;
    bool retVal = false;
    while (pData->iter != pData->res.end() && retVal == false)
    {
        bsoncxx::document::view data = *pData->iter;
        if (check_document(data) == false)
        {
            pData->iter++;
            continue;
        }

        retVal = true;
    }
    return retVal;
}

void mongodb_iterRelease(corto_iter *iter)
{
    mongodb_iterData *data = (mongodb_iterData*)iter->udata;
    delete data;
}

/* $end */
corto_resultIter _mongo_Connector_onRequest(
    mongo_Connector _this,
    corto_request *request)
{
/* $begin(recorto/mongo/Connector/onRequest) */
    corto_resultIter result;

    std::string parent = SAFE_STRING(request->parent);
    std::string expr = SAFE_STRING(request->expr);
    std::string type = SAFE_STRING(request->type);

    //uint64_t offset = request->offset;
    //uint64_t limit = request->limit;
    //bool content = request->content;

    CMongoPool *pPool = (CMongoPool*)corto_olsGet(_this, MONGOPOOL_HANDLE);
    mongodb_iterData *data = (mongodb_iterData*)corto_calloc(sizeof(mongodb_iterData));
    new (&data->client) MongoClientPtr();
    new (&data->parent) std::string();

    data->client = pPool->GetClient();
    data->parent = parent;
    if (parent == ".")
    {
        parent = "/";
    }
    mongocxx::collection coll = (*data->client)[_this->dbname][parent];
    if (expr.find("*") != std::string::npos)
    {
        if (expr == "*")
        {
            data->res = coll.find({});
            data->iter = data->res.begin();
        }
        else
        {
            bsoncxx::builder::stream::document filter_builder;
            filter_builder << "id" << bsoncxx::types::b_regex(expr,"i");

            data->res = coll.find(filter_builder.view());
            data->iter = data->res.begin();
        }
    }
    else
    {
        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "id" << expr;

        data->res = coll.find(filter_builder.view());
        data->iter = data->res.begin();
    }

    data->result.id = NULL;
    data->result.name = NULL;
    data->result.parent = NULL;
    data->result.type = NULL;
    data->result.value = 0;
    data->result.leaf = false;

    result.udata = data;
    result.hasNext = mongodb_iterHasNext;
    result.next = mongodb_iterNext;
    result.release = mongodb_iterRelease;

    return result;
/* $end */
}
