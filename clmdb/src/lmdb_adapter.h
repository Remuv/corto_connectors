#ifndef __CLMDB_LMDB_ADAPTER_H__
#define __CLMDB_LMDB_ADAPTER_H__

#include <common/vendor/lmdb/lmdb.h>

#include <unordered_map>
#include <thread>
#include <string>
#include <mutex>
#include <csignal>

namespace LMDB
{
    struct StringHasher
    {
        std::size_t operator()(const std::string &k) const;
    };

    struct StringEquals
    {
        bool operator()(const std::string &lhs, const std::string &rhs) const;
    };

    struct Data
    {
        std::string m_key;
        void        *m_data;
        size_t      m_size;
        Data() : m_data(nullptr), m_size(0){}
    };

    class Cursor
    {
    private:
        friend class MDBEnv;

        MDB_cursor *m_cursor;

        bool        m_hasNext;
        bool        m_single;
        std::string m_name;
        std::string m_data;

        Cursor();

    public:
        Cursor(Cursor &&c);

        ~Cursor();

        bool Begin();

        Data GetData();

        bool HasNext();

        bool Next();
    };

    class MDBEnv
    {
    private:
        struct UpdateEvent
        {
            std::string m_parent;
            std::string m_id;
            std::string m_data;
        };

        template<typename T>
        using RinMap = std::unordered_map<std::string, T, StringHasher, StringEquals>;

        typedef RinMap<UpdateEvent> EventMap;

        MDB_env   *m_env;

        volatile sig_atomic_t m_running;
        std::thread           m_thread;
        std::mutex            m_updateMutex;
        EventMap              m_updateBuffer;
        uint64_t              m_updateRate;

        void ProcessThread();
        void ProcessEvent();
        int GetData(std::string &parent, std::string &id, std::string &out);

    public:
        void Initialize(const char *path, uint32_t flags, uint32_t mode, uint64_t mapSize, uint64_t updateRate);
        void Destroy();

        void DefineData(std::string &parent, std::string &id, std::string &data);
        void UpdateData(std::string &parent, std::string &id, std::string &data);
        void DeleteData(std::string &parent, std::string &id);

        Cursor GetCursor(std::string &parent, std::string &expr);

        MDBEnv();
        ~MDBEnv();
    };
}

#endif
