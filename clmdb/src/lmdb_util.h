#ifndef __CLMDB_LMDB_UTIL_H__
#define __CLMDB_LMDB_UTIL_H__

#include <lmdb.h>
#include <string>
#include <mutex>
#include <map>

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

    class LMDB_MAP {
     public:
        std::mutex m_mtx;
        std::map<std::string, MDB_env *> m_files;
        LMDB_MAP();
        ~LMDB_MAP();
    };

private:
    static LMDB_MAP m_factory;

public:
    static MDB_env *GetMDB(std::string path);
    static int Initialize(const char *path, uint32_t flags, mdb_mode_t mode, uint64_t map_size);

    static int SetData(std::string path, std::string db, std::string key, MDB_val &data);
    static int GetData(std::string path, std::string db, std::string key, MDB_val &out);

    static int Delete(std::string path, std::string db, std::string key);

    static Cursor GetCursor(std::string path, std::string db, std::string expr);
};

#endif
