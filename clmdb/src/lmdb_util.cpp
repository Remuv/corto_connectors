#include "lmdb_util.h"
#include <corto/corto.h>

extern corto_threadKey CLMDB_TLS_KEY;

#define TRACE() printf("TRACE: %s, %i\n", __FILE__, __LINE__)

#define SAFE_STRING(str) std::string(str != nullptr ? str : "")

struct MDB_txn_data
{
    MDB_txn *txn;
    int refCount;
};

CLMDB::Cursor::Cursor(Cursor &&c)
{
    this->m_cursor = c.m_cursor;
    this->m_single = c.m_single;
    this->m_hasNext = c.m_hasNext;
    this->m_name = c.m_name;
    this->m_data = c.m_data;

    c.m_single = false;
    c.m_hasNext = false;
    c.m_cursor = nullptr;
}

CLMDB::Cursor::Cursor() :
    m_cursor(nullptr),
    m_single(false),
    m_hasNext(false)
{

}

CLMDB::Cursor::~Cursor()
{
    if (m_cursor != nullptr)
    {
        MDB_txn_data *data = (MDB_txn_data *)corto_threadTlsGet(CLMDB_TLS_KEY);
        MDB_txn *txn = mdb_cursor_txn(m_cursor);

        mdb_cursor_close(m_cursor);
        m_cursor = nullptr;

        if (data != nullptr && data->txn == txn)
        {
            data->refCount--;
            if (data->refCount == 0)
            {
                mdb_txn_commit(data->txn);
                data->txn = nullptr;
            }
        }
        else
        {
            printf("Wrong txn on Tls\n");
        }
    }
}

bool CLMDB::Cursor::Begin()
{
    bool retVal = false;

    if (m_cursor != nullptr)
    {
        MDB_val key;
        MDB_val data;
        retVal = mdb_cursor_get(m_cursor, &key, &data, MDB_FIRST) == 0;
        m_hasNext = retVal;
    }

    return retVal;
}

CLMDB::Cursor::Data CLMDB::Cursor::GetData()
{
    Data retVal;
    if (m_single == true)
    {
        if (m_hasNext == true)
        {
            retVal.key = m_name;
            retVal.data = m_data.mv_data;
            retVal.size = m_data.mv_size;
        }
    }
    else if (m_cursor != nullptr)
    {
        int rc = 0;
        MDB_val key;
        MDB_val data;

        rc = mdb_cursor_get(m_cursor, &key, &data, MDB_GET_CURRENT);

        if (rc == 0)
        {
            retVal.key = std::string((char*)key.mv_data, key.mv_size);

            retVal.data = data.mv_data;
            retVal.size = data.mv_size;
        }
        else
        {

        }
    }

    return retVal;
}
CLMDB::LMDB_MAP CLMDB::m_factory;

bool CLMDB::Cursor::HasNext()
{
    bool retVal = m_hasNext;

    return retVal;
}

bool CLMDB::Cursor::Next()
{
    bool retVal = false;
    if (m_single)
    {
        m_hasNext = false;
    }
    else if (m_cursor != nullptr)
    {
        MDB_val key;
        MDB_val data;

        int rc = mdb_cursor_get(m_cursor, &key, &data, MDB_NEXT);

        retVal = rc == 0;
        m_hasNext = retVal;
    }

    return retVal;
}


CLMDB::LMDB_MAP::LMDB_MAP()
{

}

CLMDB::LMDB_MAP::~LMDB_MAP()
{
    std::map<std::string, MDB_env*>::iterator iter;

    while (m_files.empty() == false)
    {
        iter = m_files.begin();
        mdb_env_close(iter->second);
        m_files.erase(iter);
    }
}

MDB_env *CLMDB::GetMDB(std::string path)
{
    MDB_env *env = m_factory.m_files[path];

    return env;
}

int CLMDB::Initialize(const char *path, uint32_t flags, mdb_mode_t mode, uint64_t map_size)
{
    int retCode = -1;

    std::string key = SAFE_STRING(path);

    MDB_env *env = m_factory.m_files[key];

    if (env == nullptr)
    {
        if (mdb_env_create(&env) == 0)
        {
            if (mdb_env_set_mapsize(env, map_size) == 0 &&
                mdb_env_set_maxdbs(env, 32767) == 0)
            {
                int rc = mdb_env_open(env, path, flags, mode);
                //if (mdb_env_open(env, path, flags, mode) == 0)
                if (rc == 0)
                {
                    m_factory.m_files[key] = env;
                    retCode = 0;
                }
                else
                {
                    mdb_env_close(env);
                    env = nullptr;
                }
            }
            else
            {
                mdb_env_close(env);
                env = nullptr;
            }
        }
        else
        {
            env = nullptr;
        }
    }
    else
    {
        retCode = 0;
    }
    return retCode;
}

int CLMDB::SetData(std::string path, std::string db, std::string key, MDB_val &data)
{
    int retCode = -1;

    MDB_env *m_env = CLMDB::GetMDB(path);

    if (m_env != nullptr)
    {
        MDB_txn *txn = nullptr;

        if (mdb_txn_begin(m_env, nullptr, 0, &txn) == 0)
        {
            MDB_dbi dbi = 0;
            if( mdb_dbi_open(txn, db.c_str(), MDB_CREATE, &dbi) == 0)
            {
                MDB_val key_v;
                key_v.mv_size = key.size();
                key_v.mv_data = (void*)key.data();

                if (mdb_put(txn, dbi, &key_v, &data, 0) == 0)
                {
                    retCode = mdb_txn_commit(txn);
                }
                else
                {
                    mdb_txn_abort(txn);
                }
            }
            else
            {
                mdb_txn_abort(txn);
            }
        }
    }
    return retCode;
}

int CLMDB::GetData(std::string path, std::string db, std::string key, MDB_val &out)
{
    int retCode = -1;
    MDB_env *m_env = CLMDB::GetMDB(path);

    if (m_env != nullptr)
    {
        MDB_txn *txn = nullptr;

        if (mdb_txn_begin(m_env, nullptr, MDB_RDONLY, &txn) == 0)
        {
            MDB_dbi dbi = 0;
            if (mdb_dbi_open(txn, db.c_str(), MDB_CREATE, &dbi) == 0)
            {
                MDB_val key_v;
                key_v.mv_size = key.size();
                key_v.mv_data = (void*)key.data();
                retCode = mdb_get(txn, dbi, &key_v, &out);
                if (retCode != 0)
                {
                    mdb_txn_abort(txn);
                }
                else
                {
                    mdb_txn_commit(txn);
                }
            }
            else
            {
                mdb_txn_abort(txn);
            }
        }
    }
    return retCode;
}

int CLMDB::Delete(std::string path, std::string db, std::string key)
{
    int retCode = -1;

    MDB_env *m_env = CLMDB::GetMDB(path);

    if (m_env != nullptr)
    {
        MDB_txn *txn = nullptr;

        if (mdb_txn_begin(m_env, nullptr, 0, &txn) == 0)
        {
            MDB_dbi dbi = 0;
            if (mdb_dbi_open(txn, db.c_str(), MDB_CREATE, &dbi) == 0)
            {
                MDB_val key_v;
                MDB_val data;
                key_v.mv_size = key.size();
                key_v.mv_data = (void*)key.data();

                if (mdb_del(txn, dbi, &key_v, &data) == 0)
                {
                    retCode = mdb_txn_commit(txn);
                }
                else
                {
                    mdb_txn_abort(txn);
                }
            }
            else
            {
                mdb_txn_abort(txn);
            }
        }
    }
    return retCode;
}

CLMDB::Cursor CLMDB::GetCursor(std::string path, std::string db, std::string expr)
{
    Cursor retVal;

    if (expr.find("*") != std::string::npos)
    {
        MDB_env *m_env = CLMDB::GetMDB(path);

        if (m_env != nullptr)
        {
            MDB_txn_data *data = (MDB_txn_data*)corto_threadTlsGet(CLMDB_TLS_KEY);

            if (data == nullptr)
            {
                data = (MDB_txn_data*)malloc(sizeof(MDB_txn_data));
                data->txn = nullptr;
                data->refCount = 0;
                corto_threadTlsSet(CLMDB_TLS_KEY, data);
            }

            if (data->txn == nullptr)
            {
                MDB_txn *txn = nullptr;
                if (mdb_txn_begin(m_env, nullptr, MDB_RDONLY, &txn) == 0)
                {
                    MDB_dbi dbi = 0;
                    if (mdb_dbi_open(txn, db.c_str(), 0, &dbi) == 0)
                    {
                        MDB_cursor *cur;
                        if (mdb_cursor_open(txn, dbi, &cur) == 0)
                        {
                            data->txn = txn;
                            data->refCount = 1;

                            retVal.m_cursor = cur;
                            retVal.Begin();

                        }
                        else
                        {
                            mdb_txn_abort(txn);
                        }
                    }
                    else
                    {
                        mdb_txn_abort(txn);
                    }
                }
            }
            else
            {
                MDB_dbi dbi = 0;
                if (mdb_dbi_open(data->txn, db.c_str(), 0, &dbi) == 0)
                {
                    MDB_cursor *cur;
                    if (mdb_cursor_open(data->txn, dbi, &cur) == 0)
                    {
                        data->refCount++;
                        retVal.m_cursor = cur;
                        retVal.Begin();

                    }
                }
            }
        }
    }
    else
    {
        retVal.m_single = true;

        int rc = GetData(path, db, expr, retVal.m_data);

        if (rc == 0)
        {
            retVal.m_hasNext = true;
            retVal.m_name = expr;
        }
    }

    return std::move(retVal);
}
