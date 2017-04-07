/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/mongo_connector/mongo/mongo.h>

/* $header() */
#include "mongo_util.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/builder/core.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/pipeline.hpp>

using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using bsoncxx::types::b_date;
using bsoncxx::types::b_array;
using bsoncxx::document::element;

/* ********************************
 * MONGO SYNC
 * ********************************/

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

    StrToLower(name);
    StrToLower(collection);

    try
    {
        CMongoPool *pPool = (CMongoPool*)_this->mongo_handle;
        if (pPool == nullptr)
        {
            corto_error("Invalid mongo pool handle.");
        }

        std::string parent_s = collection;

        if (collection == ".") {
            collection = "/";
        }

        MongoClientPtr pClient = pPool->GetClient();
        if (pClient.get() == nullptr)
        {
            corto_error("Invalid mongo pool client.");
        }
        mongocxx::collection coll = (*pClient)[database][collection];

        bsoncxx::builder::stream::document filterBuilder;
        filterBuilder << "id" << name;

        auto cursor = coll.find_one(filterBuilder.view());
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
            coll.update_one(filterBuilder.view(), update_builder.view());
        }
    }
    catch(std::exception &e)
    {
        corto_error("%s", e.what());
    }
}

void StringFindReplace(std::string& source,
                       std::string const& find,
                       std::string const& replace)
{
    for(std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
    {
        source.replace(i, find.length(), replace);
        i += replace.length();
    }
}

void mongo_Connector_delete(
    mongo_Connector _this,
    corto_string parent,
    corto_string id
)
{
    std::string databaseStr = SAFE_STRING(_this->dbname);
    std::string collectionStr = SAFE_STRING(parent);
    std::string name = SAFE_STRING(id);

    StrToLower(name);
    StrToLower(collectionStr);

    try
    {
        CMongoPool *pPool = (CMongoPool*)_this->mongo_handle;

        if (collectionStr == ".") {
            collectionStr = "/";
        }

        MongoClientPtr pClient = pPool->GetClient();
        mongocxx::database database = (*pClient)[databaseStr];
        mongocxx::collection collection = database[collectionStr];

        bsoncxx::builder::stream::document filterBuilder;
        filterBuilder << "id" << name;

        collection.delete_many(filterBuilder.view());

        std::string escapedStr(collectionStr+"/"+id);
        StringFindReplace(escapedStr, "/", "\\/");

        std::string regex("");
        std::string option("si");
        regex.append(escapedStr);
        regex.append("");
        bsoncxx::builder::stream::document collFilterBuilder;
        collFilterBuilder << "name" << bsoncxx::types::b_regex(regex, option);

        mongocxx::cursor collCursor = database.list_collections(collFilterBuilder.view());
        for (bsoncxx::document::view doc : collCursor)
        {
            element nameElement = doc["name"];
            if (nameElement.type() == bsoncxx::type::k_utf8)
            {
                bsoncxx::types::b_utf8 utfStr = nameElement.get_utf8();
                std::string docName = utfStr.value.to_string();

                mongocxx::collection childColl = database[docName];
                childColl.drop();
                corto_debug("Drop Collection [%s] from [%s]", docName.c_str(), databaseStr.c_str());
            }
        }
    }
    catch(std::exception &e)
    {
        corto_error("%s", e.what());
    }
}

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
    mongocxx::cursor cursor;
    mongocxx::cursor::iterator iter;

    corto_result result;
};

void *mongodb_iterDataNext(corto_iter *iter)
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

    corto_setstr(&pData->result.parent, ".");

    pData->result.name = NULL;
    pData->iter++;

    return &pData->result;
}

int mongodb_iterDataHasNext(corto_iter *iter)
{
    bool retVal = false;

    mongodb_iterData *pData = (mongodb_iterData*)iter->udata;

    if (pData == nullptr)
    {
        corto_error("MongoDB iterator is uninitialized.");
        return false;
    }

    while (retVal == false && pData->iter != pData->cursor.end())
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

void mongodb_iterDataRelease(corto_iter *iter)
{
    if (iter->udata != NULLWORD)
    {
        mongodb_iterData *pData = (mongodb_iterData*)iter->udata;

        if (pData == nullptr)
        {
            corto_error("MongoDB iterator is uninitialized.");
        }

        typedef mongocxx::cursor Cursor;
        typedef mongocxx::cursor::iterator Iterator;

        pData->client.~MongoClientPtr();
        pData->cursor.~Cursor();
        pData->iter.~Iterator();

        free(pData);

        iter->udata = NULLWORD;
    }
}

mongodb_iterData *mongodb_iterDataNew(MongoClientPtr &&pClient, mongocxx::cursor &&cursor)
{
    mongodb_iterData *pData = (mongodb_iterData*)calloc(1, sizeof(mongodb_iterData));
    new (&pData->client) MongoClientPtr(std::move(pClient));
    new (&pData->cursor) mongocxx::cursor(std::move(cursor));
    new (&pData->iter) mongocxx::cursor::iterator(pData->cursor.begin());
    return pData;
}
/* $end */

corto_int16 _mongo_Connector_construct(
    mongo_Connector _this)
{
/* $begin(recorto/mongo_connector/mongo/Connector/construct) */
    CMongoPool *pPool = new CMongoPool();

    pPool->Initialize(SAFE_STRING(_this->user),
                        SAFE_STRING(_this->password),
                        SAFE_STRING(_this->hostaddr),
                        std::to_string(_this->port));
    _this->mongo_handle = (corto_word)pPool;

    corto_mount_setContentType(_this, "text/json");
    corto_mount(_this)->kind = CORTO_SINK;
    return corto_mount_construct(_this);
/* $end */
}

corto_void _mongo_Connector_destruct(
    mongo_Connector _this)
{
/* $begin(recorto/mongo_connector/mongo/Connector/destruct) */
    if (_this->mongo_handle != NULLWORD)
    {
        CMongoPool *pPool = (CMongoPool*)_this->mongo_handle;
        delete pPool;
    }
    corto_mount_destruct(_this);
/* $end */
}

corto_void _mongo_Connector_onNotify(
    mongo_Connector _this,
    corto_eventMask event,
    corto_result *object)
{
/* $begin(recorto/mongo_connector/mongo/Connector/onNotify) */
    if (_this->mongo_handle == NULLWORD)
    {
        return;
    }

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

corto_resultIter _mongo_Connector_onRequest(
    mongo_Connector _this,
    corto_request *request)
{
/* $begin(recorto/mongo_connector/mongo/Connector/onRequest) */
    if (_this->mongo_handle == NULLWORD)
    {
        corto_resultIter result;
        return result;
    }

    corto_resultIter result;

    std::string parent = SAFE_STRING(request->parent);
    std::string expr = SAFE_STRING(request->expr);
    std::string type = SAFE_STRING(request->type);

    StrToLower(expr);
    StrToLower(parent);

    if (parent == ".")
    {
        parent = "/";
    }

    bsoncxx::builder::stream::document filterBuilder;

    if (expr.find("*") == std::string::npos)
    {
        filterBuilder << "id" << expr;
    }
    else if (expr != "*")
    {
        filterBuilder << "id" << bsoncxx::types::b_regex(expr,"i");
    }

    mongodb_iterData *pData = nullptr;
    CMongoPool *pPool =  (CMongoPool*)_this->mongo_handle;

    try
    {
        MongoClientPtr pClient = pPool->GetClient();
        mongocxx::collection coll = (*pClient)[_this->dbname][parent];
        auto res = coll.find(filterBuilder.view());
        pData = mongodb_iterDataNew(std::move(pClient), std::move(res));
    }
    catch(std::exception &e)
    {
        corto_error("%s", e.what());
    }

    result.udata = pData;
    result.hasNext = mongodb_iterDataHasNext;
    result.next = mongodb_iterDataNext;
    result.release = mongodb_iterDataRelease;

    return result;
/* $end */
}
