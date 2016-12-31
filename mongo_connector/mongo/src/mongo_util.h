#ifndef __MONGO_MONGO_UTIL_H__
#define __MONGO_MONGO_UTIL_H__

#include "mongo.h"

#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>

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

#endif
