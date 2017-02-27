#ifndef __MONGO_MONGO_UTIL_H__
#define __MONGO_MONGO_UTIL_H__

#include "mongo.h"

#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>

#define SAFE_DELETE(ptr) if (ptr != nullptr) {delete ptr; ptr = nullptr;}
#define SAFE_STRING(str) std::string(str != nullptr ? str : "")
#define TO_LOWER(c) if (c <= 'Z' && c >= 'A') c += 32
#define NULLWORD 0

typedef mongocxx::pool::entry MongoClientPtr;
typedef std::unique_ptr<mongocxx::pool> MongoPoolPtr;

class CMongoPool
{
private:
    MongoPoolPtr m_pMongoPool;

public:
    CMongoPool();
    ~CMongoPool();

    void Initialize(std::string user,
                    std::string password,
                    std::string hostaddr,
                    std::string port);

    MongoClientPtr GetClient();
};

template <typename T>
class optional
{
private:
    bool m_hasValue;
    T    m_value;

public:
    optional () : m_hasValue(false) {}
    optional (const T &v) : m_hasValue(true), m_value(v) {}
    optional (T &&v) : m_hasValue(true), m_value(std::move(v)) {}
    optional (const optional &o) : m_hasValue(o.m_hasValue), m_value(o.m_value) {}

    T &value ()
    {
        return m_value;
    }

    void setValue(const T &value)
    {
        m_value = value;
        m_hasValue = true;
    }

    operator bool ()
    {
        return m_hasValue;
    }
};

void StrToLower(std::string &str);

#endif
