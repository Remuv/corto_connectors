#include <recorto/mongo_connector/mongo/mongo_util.h>

CMongoPool::CMongoPool() : m_pMongoPool(nullptr)
{

}

CMongoPool::~CMongoPool()
{
    m_pMongoPool.reset(nullptr);
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

void StrToLower(std::string &str)
{
    size_t size = str.size();
    char *c_str = &str[0];
    for (size_t i = 0; i < size; i++)
    {
        char &c = c_str[i];
        TO_LOWER(c);
    }
}
