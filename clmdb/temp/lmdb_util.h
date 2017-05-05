#ifndef __CLMDB_LMDB_UTIL_H__
#define __CLMDB_LMDB_UTIL_H__

#include <common/vendor/lmdb/lmdb.h>
#include <string>
#include <mutex>
#include <map>


struct StringHasher
{
    std::size_t operator()(const std::string &k) const;
};

struct StringEquals
{
    bool operator()(const std::string &lhs, const std::string &rhs) const;
};

class CLMDB
{
public:
    class Cursor
    {
     private:
        friend class CLMDB;
        MDB_cursor *m_cursor;

        std::string m_name;
        MDB_val     m_data;
        bool        m_single;
        bool        m_hasNext;

        Cursor();
     public:
        struct Data{
            std::string key;
            void *data;
            size_t size;
        };

        Cursor(Cursor &&c);

        ~Cursor();

        bool Begin();

        Data GetData();

        bool HasNext();

        bool Next();
    };

private:
    template<typename T>
    using RinMap = std::unordered_map<std::string, T, StringHasher, StringEquals>;

    class LMDB_MAP
    {
     public:
        std::mutex m_mtx;
        std::map<std::string, MDB_env *> m_files;
        LMDB_MAP();
        ~LMDB_MAP();
    };

    class Event
    {
        enum Type
        {
            NONE,
            UDPATE,
            DELETE
        };

        Type m_event;
        std::string m_path;
        std::string m_db;
        std::string m_key;
        std::string m_data;

        Event():m_event(Type::NONE){}
    }

    typedef RinMap<Event> EventMap;

    static std::mutex m_ebMtx;
    static BufferMap  m_eventBuffer;

    static LMDB_MAP   m_factory;

    static void ProcessEvent();

    static void SetData(Event &event);
    static void DelData(Event &event);

    static MDB_env *GetMDB(std::string path);
    static void GetData(std::string &path, std::string &db, std::string &key, std::string &out);
    static void FreeData(MDB_val &data);

public:
    static void Initialize(const char *path, uint32_t flags, mdb_mode_t mode, uint64_t map_size);

    static void DefData(std::string &path, std::string &db, std::string &key, std::string &data);
    static void SetData(std::string &path, std::string &db, std::string &key, std::string &data);
    static void DelData(std::string &path, std::string &db, std::string &key);

    static Cursor GetCursor(std::string path, std::string db, std::string expr);
};

#endif
