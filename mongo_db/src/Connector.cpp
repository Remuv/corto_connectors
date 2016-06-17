/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/mongodb/mongodb.h>

/* $header() */
#include "mongo_util.h"
#include "json_util.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/array.hpp>

extern corto_uint8 MONGOCLIENT_HANDLE;

struct mongodb_iterData
{
    mongocxx::cursor res;
    mongocxx::cursor::iterator iter;
    corto_result result;
    char *type;
    char *key;
};

void *mongodb_iterNext(corto_iter *iter)
{
    mongodb_iterData *data = (mongodb_iterData*)iter->udata;

    std::string json       = bsoncxx::to_json(*data->iter);
    JSON_Value  *jsonValue = json_parse_string(json.c_str());
    JSON_Object *o         = json_value_get_object(jsonValue);
    JSON_Value  *v         = json_object_get_value(o,data->key);
    const char  *s         = json_value_get_string(v);

    corto_setstr(&data->result.id, (char*)s);
    corto_setstr(&data->result.name, (char*)s);
    corto_setstr(&data->result.type, (char*)data->type);
    data->result.parent = ".";
    data->result.value  = (corto_word)corto_strdup((char*)json.c_str());
    if (jsonValue != NULL)
    {
        json_value_free(jsonValue);
    }

    data->iter++;

    return &data->result;
}

int mongodb_iterHasNext(corto_iter *iter)
{
    mongodb_iterData *data = (mongodb_iterData*)iter->udata;
    return data->iter != data->res.end();
}

void mongodb_iterRelease(corto_iter *iter)
{
    mongodb_iterData *data = (mongodb_iterData*)iter->udata;
    delete data->type;
    delete data->key;
    delete data;
}

void mongodb_ConnectorUpdateDb(
    mongodb_Connector _this,
    corto_object observable)
{
    corto_type obj_type = (corto_type)corto_resolve(NULL, _this->objtype);
    if ( obj_type == nullptr || corto_instanceof(obj_type, observable) == false)
    {
        corto_seterr("Observable not instance of %s", _this->objtype);
        return;
    }

    corto_member mkey = corto_interface_resolveMember(obj_type, _this->keyname);
    if (mkey == nullptr)
    {
        corto_seterr("%s is not a member of %s", _this->keyname, _this->objtype);
        return;
    }
    if (mkey->type != corto_type(corto_string_o))
    {
        corto_seterr("member key(%s) must be of type corto_string", _this->keyname);
        return;
    }
    corto_string *key = (corto_string*)CORTO_OFFSET(observable, mkey->offset);

    corto_id path;
    corto_path(path, corto_mount(_this)->mount, observable, ".");

    char *ptr = path, ch;
    while ((ch = *ptr))
    {
        if (ch == '/')
        {
            *ptr = '.';
        }
        ptr++;
    }
    corto_setstr(key, (char*)path);

    CMongoClient *p_client = (CMongoClient*)corto_olsGet(_this, MONGOCLIENT_HANDLE);
    mongocxx::collection coll = p_client->conn[_this->dbname][_this->collection];

    bsoncxx::builder::stream::document filter_builder;

    char * json = mongo_json::json_serialize(observable);
    std::string json_obj = std::string(json);
    corto_dealloc(json);

    filter_builder << std::string(_this->keyname) << std::string(*key);

    auto curso = coll.find_one(filter_builder.view());
    if (!curso)
    {
        bsoncxx::document::value doc = bsoncxx::from_json(json_obj);
        auto result = coll.insert_one(doc.view());

        corto_member mid = corto_interface_resolveMember(obj_type, "_id");
        if (mid != NULL && result && mid->type == corto_type(corto_string_o))
        {
            std::string oid_s = result->inserted_id().get_oid().value.to_string();
            corto_string *id = (corto_string*)CORTO_OFFSET(observable, mid->offset);
            corto_setstr(id, (char*)oid_s.c_str());
        }
    }
    else
    {
        bsoncxx::document::value val = bsoncxx::from_json(json_obj);
        auto result = coll.replace_one(filter_builder.view(), val.view());

        corto_member mid = corto_interface_resolveMember(obj_type, "_id");
        if (mid != NULL && result && result->upserted_id() && mid->type == corto_type(corto_string_o))
        {
            std::string oid_s = (*result->upserted_id())["_id"].get_oid().value.to_string();
            corto_string *id = (corto_string*)CORTO_OFFSET(observable, mid->offset);
            corto_setstr(id, (char*)oid_s.c_str());
        }
    }
}

/* $end */

corto_int16 _mongodb_Connector_construct(
    mongodb_Connector _this)
{
/* $begin(recorto/mongodb/Connector/construct) */

    if (!corto_checkAttr(_this, CORTO_ATTR_SCOPED))
    {
        corto_seterr("mongodb/Connector objects must be SCOPED");
        return -1;
    }

    corto_type obj_type = (corto_type)corto_resolve(NULL, _this->objtype);
    if (obj_type == NULL)
    {
        corto_seterr("Unable to resolve for object type: %s", _this->objtype);
        return -1;
    }

    corto_member mkey = corto_interface_resolveMember(obj_type, _this->keyname);
    if (mkey == NULL)
    {
        corto_seterr("%s does not have member %s", _this->objtype, _this->keyname);
        return -1;
    }

    if (mkey->type != corto_type(corto_string_o))
    {
        corto_seterr("Member Key %s must be of type corto_string", _this->keyname);
        return -1;
    }

    CMongoClient *p_client = new CMongoClient(_this->user, _this->password, _this->hostaddr, std::to_string(_this->port));

    corto_olsSet(_this, MONGOCLIENT_HANDLE, p_client);

    corto_mount_setContentType(_this, "text/json");
    corto_mount(_this)->kind = CORTO_SINK;
    return corto_mount_construct(_this);

/* $end */
}

corto_string _mongodb_Connector_FindById(
    mongodb_Connector _this,
    corto_string id)
{
/* $begin(recorto/mongodb/Connector/FindById) */

    CMongoClient *p_client = (CMongoClient*)corto_olsGet(_this, MONGOCLIENT_HANDLE);
    mongocxx::collection coll = p_client->conn[_this->dbname][_this->collection];

    bsoncxx::builder::stream::document filter_builder;
    filter_builder << "_id" << bsoncxx::oid(std::string(id));

    mongocxx::cursor cursor = coll.find(filter_builder.view());

    for (mongocxx::cursor::iterator itr = cursor.begin(); itr != cursor.end(); itr++)
    {
        corto_string json = corto_strdup((char*)bsoncxx::to_json(*itr).c_str());
        return json;
    }
    return NULL;

/* $end */
}

mongodb_stringList _mongodb_Connector_FindByIds(
    mongodb_Connector _this,
    mongodb_stringList ids)
{
/* $begin(recorto/mongodb/Connector/FindByIds) */

    mongodb_stringList list = corto_llNew();

    CMongoClient *p_client = (CMongoClient*)corto_olsGet(_this, MONGOCLIENT_HANDLE);
    mongocxx::collection coll = p_client->conn[_this->dbname][_this->collection];

    bsoncxx::builder::stream::array arrayIds;
    mongodb_stringListForeach(ids, id)
    {
        arrayIds << bsoncxx::oid(std::string(id));
    }
    bsoncxx::builder::stream::document builder;
    builder << "_id" << bsoncxx::builder::stream::open_document << "$in"
            << bsoncxx::builder::stream::open_array << bsoncxx::builder::concatenate_array{arrayIds.view()} << bsoncxx::builder::stream::close_array
            << bsoncxx::builder::stream::close_document;

    mongocxx::cursor cursor = coll.find(builder.view());
    for (mongocxx::cursor::iterator itr = cursor.begin(); itr != cursor.end(); itr++)
    {
        corto_string json = corto_strdup((char*)bsoncxx::to_json(*itr).c_str());
        corto_llInsert(list, json);
    }
    return list;

/* $end */
}

corto_void _mongodb_Connector_onDeclare(
    mongodb_Connector _this,
    corto_object observable)
{
/* $begin(recorto/mongodb/Connector/onDeclare) */


/* $end */
}

corto_void _mongodb_Connector_onDelete(
    mongodb_Connector _this,
    corto_object observable)
{
/* $begin(recorto/mongodb/Connector/onDelete) */

    CMongoClient *p_client = (CMongoClient*)corto_olsGet(_this, MONGOCLIENT_HANDLE);
    mongocxx::collection coll = p_client->conn[_this->dbname][_this->collection];

    corto_id buffer;
    corto_string name = corto_path(buffer, corto_mount(_this)->mount, observable, ".");

    bsoncxx::builder::stream::document filter_builder;
    filter_builder << std::string(_this->keyname) << std::string(name) << bsoncxx::builder::stream::finalize;

    coll.delete_many(filter_builder.view());
/* $end */
}

corto_resultIter _mongodb_Connector_onRequest(
    mongodb_Connector _this,
    corto_request *request)
{
/* $begin(recorto/mongodb/Connector/onRequest) */
    corto_resultIter result;

    CMongoClient *p_client = (CMongoClient*)corto_olsGet(_this, MONGOCLIENT_HANDLE);
    mongodb_iterData *data = (mongodb_iterData*)corto_calloc(sizeof(mongodb_iterData));
    mongocxx::collection coll = p_client->conn[_this->dbname][_this->collection];
    data->res = coll.find({});
    data->iter = data->res.begin();
    data->key  = corto_strdup(_this->keyname);
    data->type = corto_strdup(_this->objtype);
    data->result.id = NULL;
    data->result.name = NULL;
    data->result.type = NULL;
    data->result.parent = NULL;
    data->result.value = 0;

    result.udata   = data;
    result.hasNext = mongodb_iterHasNext;
    result.next    = mongodb_iterNext;
    result.release = mongodb_iterRelease;

    return result;
/* $end */
}

corto_object _mongodb_Connector_onResume(
    mongodb_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object o)
{
/* $begin(recorto/mongodb/Connector/onResume) */
    corto_id pathb;
    corto_id path;
    sprintf(path, "%s/%s", parent, name);
    corto_cleanpath(pathb, path);

    char *ptr = pathb, ch;
    while ((ch = *ptr))
    {
        if (ch == '/')
        {
            *ptr = '.';
        }
        ptr++;
    }

    CMongoClient *p_client = (CMongoClient*)corto_olsGet(_this, MONGOCLIENT_HANDLE);
    mongocxx::collection coll = p_client->conn[_this->dbname][_this->collection];

    bsoncxx::builder::stream::document filter_builder;
    filter_builder << std::string(_this->keyname) << std::string(pathb);

    auto curso = coll.find_one(filter_builder.view());
    if (curso)
    {
        bool newObject = false;
        std::string json_obj = bsoncxx::to_json(curso.value());
        if (o == nullptr)
        {
            corto_object parent_o = corto_resolve(corto_mount(_this)->mount, parent);
            if (parent_o != nullptr)
            {
                corto_object type_o = corto_resolve(NULL, (char*)_this->objtype);
                if (type_o != nullptr)
                {
                    o = corto_declareChild(parent_o, name, (corto_type)type_o);
                    if (o == nullptr)
                    {
                        corto_seterr("Failed to create object %s/%s: %s", parent, name, corto_lasterr());
                    }
                    newObject = true;
                    corto_release(type_o);
                }
                corto_release(parent_o);
            }
        }
        if (o != nullptr)
        {
            if (mongo_json::json_deserialize(o, (char*)json_obj.c_str()))
            {
                corto_seterr("Fail: to deserialize %s/%s: %s", parent, name, corto_lasterr());
            }
            else
            {
                if(newObject)
                {
                    if (corto_define(o))
                    {
                        corto_seterr("Failed to define object %s/%s: %s", parent, name, corto_lasterr());
                    }
                }

                corto_type obj_type = (corto_type)corto_resolve(NULL, (char*)_this->objtype);
                if (obj_type !=nullptr)
                {
                    corto_member mid = corto_interface_resolveMember(obj_type, "_id");
                    if (mid != NULL && mid->type == corto_type(corto_string_o))
                    {
                        std::string oid_s = curso->view()["_id"].get_oid().value.to_string();
                        corto_string *id = (corto_string*)CORTO_OFFSET(o, mid->offset);
                        corto_setstr(id, (char*)oid_s.c_str());
                    }
                    corto_release(obj_type);
                }
            }
        }
    } else {
        if (o != nullptr)
        {
            mongodb_ConnectorUpdateDb(_this, o);
        }
        else
        {
            corto_object parent_o = corto_resolve(corto_mount(_this)->mount, parent);
            if (parent_o != nullptr)
            {
                corto_object type_o = corto_resolve(NULL, (char*)_this->objtype);
                if (type_o != nullptr)
                {
                    o = corto_declareChild(parent_o, name, (corto_type)type_o);
                    if (o == nullptr)
                    {
                        corto_seterr("Failed to create object %s/%s: %s", parent, name, corto_lasterr());
                    }
                    corto_release(type_o);
                }
                corto_release(parent_o);
            }
        }
    }
    return o;
/* $end */
}

corto_void _mongodb_Connector_onUpdate(
    mongodb_Connector _this,
    corto_object observable)
{
/* $begin(recorto/mongodb/Connector/onUpdate) */
    mongodb_ConnectorUpdateDb(_this, observable);
/* $end */
}
