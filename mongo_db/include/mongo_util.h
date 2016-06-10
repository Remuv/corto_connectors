#ifndef __MONGODB_MONGO_UTIL_H__
#define __MONGODB_MONGO_UTIL_H__

#include "mongodb.h"
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

class CMongoClient
{
public:
    mongocxx::instance inst;
    mongocxx::client conn;

    CMongoClient(std::string user, std::string password, std::string hostaddr, std::string port) : inst()
    {
        std::string auth = "";
        if (!user.empty())
        {
            auth = user+":"+password+"@";
        }
        std::string uri = "mongodb://"+auth+hostaddr+":"+port;
        conn = mongocxx::client(mongocxx::uri(uri));
    }
};

void MongodbFind(mongodb_Connector _this, std::string filter);

#endif
