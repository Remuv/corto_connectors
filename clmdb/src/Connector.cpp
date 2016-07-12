/* $CORTO_GENERATED
 *
 * Connector.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/clmdb/clmdb.h>

/* $header() */
#include <lmdb.h>
#include <string>

#define corto_trace_error(msg, args...) corto_error("%s:%i: " msg, __FILE__, __LINE__, ##args)

//convert from address to MDB_env *
#define lmdb_env(ptr) ((MDB_env*)ptr)
#define NULLWORD 0

MDB_env *create_env(const char * path, unsigned int flags, mdb_mode_t mode, unsigned long map_size)
{
    int rc = 0;
    MDB_env *ret = nullptr;
    rc = mdb_env_create(&ret);

    if (rc == 0)
    {
        rc = mdb_env_set_mapsize(ret, map_size);
        if (rc != 0)
        {
            corto_trace_error("Failed to set mapsize to %lu, error code %i",map_size, rc);
            mdb_env_close(ret);
            ret = nullptr;
        }
        else
        {
            rc = mdb_env_open(ret, path, flags, mode);
            if (rc != 0)
            {
                corto_trace_error("Failed to open env %s, error code %i", path, rc);
                mdb_env_close(ret);
                ret = nullptr;
            }
        }
    }
    else
    {
        corto_trace_error("Failed to mdb_env_create %s, error code %i",rc);
    }
    return ret;
}
/* $end */

corto_int16 _clmdb_Connector_construct(
    clmdb_Connector _this)
{
/* $begin(recorto/clmdb/Connector/construct) */
    MDB_env *handle = create_env(_this->path, _this->flags, _this->mode, _this->map_size);
    if (handle != nullptr)
    {
        _this->handle = (corto_word)handle;
    }

    corto_mount(_this)->kind = CORTO_SINK;
    return corto_mount_construct(_this);
/* $end */
}

corto_void _clmdb_Connector_destruct(
    clmdb_Connector _this)
{
/* $begin(recorto/clmdb/Connector/destruct) */
    if (_this->handle != NULLWORD)
    {
        mdb_env_close(lmdb_env(_this->handle));
        _this->handle = NULLWORD;
    }
/* $end */
}

corto_void _clmdb_Connector_onDelete(
    clmdb_Connector _this,
    corto_object observable)
{
/* $begin(recorto/clmdb/Connector/onDelete) */

/* $end */
}

corto_object _clmdb_Connector_onResume(
    clmdb_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object obj)
{
/* $begin(recorto/clmdb/Connector/onResume) */
    if (_this->handle == NULLWORD)
    {
        return obj;
    }

    corto_id path;
    sprintf(path, "%s/%s", parent, name);
    corto_cleanpath(path, path);

    char *ptr = path, ch;
    while ((ch = *ptr))
    {
        if (ch == '/')
        {
            *ptr = '.';
        }
        ptr++;
    }

    corto_lock(_this);
    int rc = 0;

    MDB_env *env = lmdb_env(_this->handle);
    MDB_txn *txn = nullptr;

    rc = mdb_txn_begin(env, nullptr, 0, &txn);
    if (rc == 0)
    {
        MDB_dbi dbi = 0;
        rc = mdb_dbi_open(txn, nullptr, 0, &dbi);
        if (rc == 0)
        {
            MDB_val key;
            MDB_val data;
            std::string key_str = std::string(path);
            key.mv_size = key_str.size();
            key.mv_data = (void*)key_str.data();

            rc = mdb_get(txn, dbi, &key, &data);
            if ( rc == 0)
            {
                //type{data}
                corto_id type;
                corto_string value;
                char *ptr = (char*)data.mv_data;
                char *ptr2 = &type[0];

                while (*ptr)
                {
                    if (*ptr == ':')
                    {
                        value = ++ptr;
                        *ptr2 = '\0';
                        break;
                    }
                    *ptr2 = *ptr;
                    ptr++;
                    ptr2++;
                }
                bool newObject = false;
                if (obj == nullptr)
                {
                    corto_object parent_o = corto_resolve(corto_mount(_this)->mount, parent);
                    if (parent_o != nullptr)
                    {
                        corto_object type_o = corto_resolve(NULL, type);
                        if (type_o != nullptr)
                         {
                            obj = corto_declareChild(parent_o, name, type_o);
                            if (obj == nullptr)
                            {
                                corto_trace_error("failed to create object %s/%s: %s",parent, name, corto_lasterr());
                            }
                            newObject = true;
                            corto_release(type_o);
                        }
                        corto_release(parent_o);
                    }
                }
                if (obj != nullptr)
                {
                    if (json_deserialize(obj, value) != 0)
                    {
                        corto_trace_error("Failed to deserialize for %s,%s: %s (%s)", type, name, corto_lasterr(), value);
                    }
                    else if (newObject)
                    {
                        if (corto_define(obj) != 0)
                        {
                            corto_trace_error("Failed to define %s", corto_idof(obj));
                        }
                    }
                }
                mdb_txn_commit(txn);
            }
            else
            {
                mdb_txn_abort(txn);
            }
        }
        else
        {
            corto_trace_error("Failed to open dbi: error code %i", rc);
            mdb_txn_abort(txn);
        }

    }
    else
    {
        corto_trace_error("Failed to open txn: error code %i", rc);
    }

    corto_unlock(_this);

    return obj;
/* $end */
}

corto_void _clmdb_Connector_onUpdate(
    clmdb_Connector _this,
    corto_object observable)
{
/* $begin(recorto/clmdb/Connector/onUpdate) */
    if (_this->handle == NULLWORD)
    {
        return;
    }
    corto_lock(_this);
    int rc = 0;

    MDB_env *env = lmdb_env(_this->handle);
    MDB_txn *txn = nullptr;

    rc = mdb_txn_begin(env, nullptr, 0, &txn);
    if (rc == 0)
    {
        MDB_dbi dbi = 0;
        rc = mdb_dbi_open(txn, nullptr, 0, &dbi);
        if (rc == 0)
        {
            std::string key_str;    //Key
            std::string data_str;   //Data = typeName:value

            corto_id typeName;
            corto_string value = nullptr;

            key_str = std::string(corto_path(nullptr, corto_mount(_this)->mount, observable, "."));

            corto_fullpath(typeName, corto_typeof(observable));
            value = json_serialize(observable);
            data_str = std::string(typeName)+":"+std::string(value);
            corto_dealloc(value);

            MDB_val key;
            MDB_val data;

            key.mv_size = key_str.size();
            key.mv_data = (void*)key_str.data();

            data.mv_size = data_str.size();
            data.mv_data = (void*)data_str.data();

            rc = mdb_put(txn, dbi, &key, &data, 0);
            if (rc == 0)
            {
                rc = mdb_txn_commit(txn);
                if (rc != 0)
                {
                    corto_trace_error("Failed to commit: error code %i", rc);
                }
            }
            else
            {
                mdb_txn_abort(txn);
                corto_trace_error("Failed to mdb put: error code %i", rc);
            }
        }
        else
        {
            corto_trace_error("Failed to open dbi: error code %i", rc);
            mdb_txn_abort(txn);
        }
    }
    else
    {
        corto_trace_error("Failed to open txn: error code %i", rc);
    }

    corto_unlock(_this);
/* $end */
}
