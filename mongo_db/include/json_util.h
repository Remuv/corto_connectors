#ifndef __CORTO_MONGODB_JSON_UTIL__
#define __CORTO_MONGODB_JSON_UTIL__
#include <corto/corto.h>
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

#endif
