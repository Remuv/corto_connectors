#include "lmdb_adapter.h"

#include <corto/corto.h>

#include <chrono>

#define MAX_WAIT_TIME 5
#define KEY(parent, id) parent+"/"+id

#define TO_LOWER(c) if (c <= 'Z' && c >= 'A') c += 32

using namespace LMDB;

typedef std::unique_lock<std::mutex> UniqueLock;

extern corto_threadKey CLMDB_TLS_KEY;

struct MDB_txn_data
{
    MDB_txn *txn;
    int refCount;
};

static void FreeData(MDB_val &data);
static void StrToLower(std::string &str);

/* *********************************
 * LMDB::StringHasher && LMDB::StringEquals
 * *********************************/

std::size_t StringHasher::operator ()(const std::string &k) const
{
    const std::size_t FNV_prime = 1099511628211;
    const std::size_t offset_basic = 14695981039346656037U;

    uint8_t c = 0;

    std::size_t hash = offset_basic;

    const char *it = k.c_str();
    while ((c = *it++) != '\0')
    {
        hash = (hash ^ c) * FNV_prime;
    }

    return hash;
}

bool StringEquals::operator ()(const std::string &lhs, const std::string &rhs) const
{
    bool retVal = lhs.size() == rhs.size();

    char c1, c2;
    const char *s1 = lhs.c_str();
    const char *s2 = lhs.c_str();

    while (retVal && (c1 = *s1++) != '\0' && (c2 = *s2++) != '\0')
    {
        retVal = c1 == c2;
    }

    return retVal;
}


/* *********************************
 * LMDB::Cursor
 * *********************************/
Cursor::Cursor() :
    m_cursor(nullptr),
    m_hasNext(false),
    m_single(false)
{

}

Cursor::Cursor(Cursor &&c)
{
    this->m_cursor = c.m_cursor;
    this->m_single = c.m_single;
    this->m_hasNext = c.m_hasNext;
    this->m_name = std::move(c.m_name);
    this->m_data = std::move(c.m_data);

    c.m_single = false;
    c.m_hasNext = false;
    c.m_cursor = nullptr;
}

Cursor::~Cursor()
{
    if (m_single == false && m_cursor != nullptr)
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
            printf("Wrong txn on Tls %p:%p\n", data!= nullptr ? data->txn : 0, txn);
        }
    }
}


bool Cursor::Begin()
{
    MDB_val key;
    MDB_val data;

    bool retVal = false;

    if (m_cursor != nullptr)
    {
        retVal = mdb_cursor_get(m_cursor, &key, &data, MDB_FIRST) == 0;
        m_hasNext = retVal;
    }

    return retVal;
}

Data Cursor::GetData()
{
    Data retVal;
    if (m_single == true)
    {
        if (m_hasNext == true)
        {
            retVal.m_key = m_name;
            retVal.m_data = (void*)m_data.data();
            retVal.m_size = m_data.size();
        }
    }
    else if (m_cursor != nullptr)
    {
        MDB_val key;
        MDB_val data;

        if (mdb_cursor_get(m_cursor, &key, &data, MDB_GET_CURRENT) == 0)
        {
            retVal.m_key = std::string((char*)key.mv_data, key.mv_size);

            retVal.m_data = data.mv_data;
            retVal.m_size = data.mv_size;
        }
    }

    return retVal;
}

bool Cursor::HasNext()
{
    return m_hasNext;
}

bool Cursor::Next()
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

        if (mdb_cursor_get(m_cursor, &key, &data, MDB_NEXT) == 0)
        {
            retVal = true;
            m_hasNext = true;
        }
    }

    return retVal;
}

/* *********************************
 * LMDB::MDBEnv
 * *********************************/

void MDBEnv::ProcessThread()
{
    while (m_running == true)
    {
        ProcessEvent();
        std::this_thread::sleep_for(std::chrono::milliseconds(m_updateRate));
    }
}

void MDBEnv::ProcessEvent()
{
    UniqueLock lock(m_updateMutex);
    EventMap updates = std::move(m_updateBuffer);
    m_updateBuffer.reserve(updates.size());
    lock.unlock();

    bool failed = false;
    MDB_txn *txn;
    MDB_dbi dbi;
    MDB_val key_v;
    MDB_val data_v;

    if (m_env != nullptr)
    {
        if (mdb_txn_begin(m_env, nullptr, 0, &txn) == 0)
        {
            for (EventMap::iterator iter = updates.begin();
                 iter != updates.end();
                 iter++)
            {
                UpdateEvent &update = iter->second;
                if (mdb_dbi_open(txn, update.m_parent.c_str(), MDB_CREATE, &dbi) == 0)
                {
                    key_v.mv_size = update.m_id.size();
                    key_v.mv_data = (void*)update.m_id.data();

                    data_v.mv_size = update.m_data.size()+1;
                    data_v.mv_data = (void*)update.m_data.data();

                    if (mdb_put(txn, dbi, &key_v, &data_v, 0) != 0)
                    {
                        failed = true;
                        break;
                    }
                }
            }

            if (failed == true)
            {
                mdb_txn_abort(txn);
            }
            else
            {
                mdb_txn_commit(txn);
            }
        }
    }
}


void MDBEnv::Initialize(const char *path, uint32_t flags, uint32_t mode, uint64_t mapSize, uint64_t updateRate)
{
    m_updateRate = updateRate;

    if (m_env == nullptr)
    {
        if (mdb_env_create(&m_env) == 0)
        {
            if (mdb_env_set_mapsize(m_env, mapSize) == 0 &&
                mdb_env_set_maxdbs(m_env, 32767) == 0)
            {
                if (mdb_env_open(m_env, path, flags, mode) != 0)
                {
                    mdb_env_close(m_env);
                    m_env = nullptr;
                }
            }
            else
            {
                mdb_env_close(m_env);
                m_env = nullptr;
            }
        }
        else
        {
            m_env = nullptr;
        }
    }

    if (m_running == false)
    {
        m_running = true;
        m_thread = std::thread(&MDBEnv::ProcessThread, this);
    }
}

void MDBEnv::Destroy()
{
    if (m_running == true)
    {
        m_running = false;

        if (m_thread.joinable() == true)
        {
            m_thread.join();
        }
    }

    if (m_env == nullptr)
    {
        mdb_env_close(m_env);
        m_env = nullptr;
    }
}

void MDBEnv::DefineData(std::string &parent, std::string &id, std::string &data)
{
    StrToLower(parent);
    StrToLower(id);

    MDB_txn *txn = nullptr;
    MDB_dbi dbi = 0;
    MDB_val key_v;
    MDB_val data_v;

    if (m_env != nullptr)
    {
        if (mdb_txn_begin(m_env, nullptr, 0, &txn) == 0)
        {
            if (mdb_dbi_open(txn, parent.c_str(), MDB_CREATE, &dbi) == 0)
            {
                key_v.mv_size = id.size();
                key_v.mv_data = (void*)id.data();

                data_v.mv_size = data.size()+1;
                data_v.mv_data = (void*)data.data();

                if (mdb_put(txn, dbi, &key_v, &data_v, 0) == 0)
                {
                    mdb_txn_commit(txn);
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
}

void MDBEnv::UpdateData(std::string &parent, std::string &id, std::string &data)
{
    StrToLower(parent);
    StrToLower(id);

    std::string key = KEY(parent, id);

    UniqueLock lock(m_updateMutex);
    UpdateEvent &uData = m_updateBuffer[key];
    uData.m_parent = std::move(parent);
    uData.m_id = std::move(id);
    uData.m_data = std::move(data);
}

void MDBEnv::DeleteData(std::string &parent, std::string &id)
{
    StrToLower(parent);
    StrToLower(id);

    MDB_txn *txn = nullptr;
    MDB_dbi dbi = 0;
    MDB_val key_v;
    MDB_val data_v;

    if (m_env != nullptr)
    {
        if (mdb_txn_begin(m_env, nullptr, 0, &txn) == 0)
        {
            if (mdb_dbi_open(txn, parent.c_str(), MDB_CREATE, &dbi) == 0)
            {
                key_v.mv_size = id.size();
                key_v.mv_data = (void*)id.data();

                if (mdb_del(txn, dbi, &key_v, &data_v) == 0)
                {
                    mdb_txn_commit(txn);
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
}

Cursor MDBEnv::GetCursor(std::string &parent, std::string &expr)
{
    StrToLower(parent);
    StrToLower(expr);

    Cursor retVal;

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
                data->txn = txn;
            }
        }

        if (data->txn != nullptr)
        {
            MDB_dbi dbi = 0;
            if (mdb_dbi_open(data->txn, parent.c_str(), 0, &dbi) == 0)
            {
                if (expr.find("*") != std::string::npos)
                {
                    MDB_cursor *cur;
                    if (mdb_cursor_open(data->txn, dbi, &cur) == 0)
                    {
                        data->refCount++;
                        retVal.m_cursor = cur;
                        retVal.Begin();
                    }
                }
                else
                {
                    MDB_val key_v;
                    MDB_val data_v;
                    key_v.mv_size = expr.size();
                    key_v.mv_data = (void*)expr.data();

                    if (mdb_get(data->txn, dbi, &key_v, &data_v) == 0)
                    {
                        retVal.m_data = std::string((char*)data_v.mv_data, data_v.mv_size);
                        retVal.m_hasNext = true;
                    }
                    else
                    {
                        retVal.m_single = true;
                    }

                    retVal.m_single = true;
                    retVal.m_name = std::move(expr);

                    if (data->refCount == 0)
                    {
                        mdb_txn_commit(data->txn);
                        data->txn = nullptr;
                    }
                }
            }
        }
    }

    return std::move(retVal);
}

MDBEnv::MDBEnv() :
    m_env(nullptr),
    m_running(false)
{

}

MDBEnv::~MDBEnv()
{
    if (m_running == true)
    {
        m_running = false;
        if (m_thread.joinable() == true)
        {
            m_thread.join();
        }
    }

    if (m_env != nullptr)
    {
        mdb_env_close(m_env);
        m_env = nullptr;
    }
}

static void FreeData(MDB_val &data)
{
    if (data.mv_data != nullptr)
    {
        free(data.mv_data);
        data.mv_data = nullptr;
        data.mv_size = 0;
    }
}

static void StrToLower(std::string &str)
{
    size_t size = str.size();
    for (size_t i = 0; i < size; i++)
    {
        char &c = str[i];
        TO_LOWER(c);
    }
}
