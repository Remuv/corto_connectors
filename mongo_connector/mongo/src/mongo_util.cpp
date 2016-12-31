#include "mongo_util.h"


CMongoPool::CMongoPool() : m_pMongoPool(nullptr)
{

}

CMongoPool::~CMongoPool()
{
    m_pMongoPool.reset();
}

void CMongoPool::Initialize(std::string user,
                std::string password,
                std::string hostaddr,
                std::string port)
{
    std::string auth = "";
    if (!user.empty())
    {
        auth = user+":"+password+"@";
    }
    std::string uri = "mongodb://"+auth+hostaddr+":"+port;
    m_pMongoPool.reset(new mongocxx::pool(mongocxx::uri(uri)));
}

MongoClientPtr CMongoPool::GetClient()
{
    return m_pMongoPool->acquire();
}
