#ifndef __MONGO_CLIENT_H__
#define __MONGO_CLIENT_H__

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

namespace mongo_json{
    /* JSON serializer data */
    typedef struct json_ser_t {
        corto_buffer buffer;
        unsigned int itemCount;
    } json_ser_t;

    corto_int16 json_deserialize(corto_object o, corto_string s);
    corto_string json_serialize(corto_object o);

    corto_string json_fromCorto(corto_object o);
    corto_int16 json_toCorto(corto_object o, corto_string json);
}

void MongodbFind(mongodb_Connector _this, std::string filter);

#endif
