/* $CORTO_GENERATED
 *
 * mongodb.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <corto/mongodb/mongodb.h>

/* $header() */
#include "mongo_util.h"

corto_uint8 MONGOCLIENT_HANDLE;

static void CMongoClientClenup(void *userData) {
    CMongoClient *client = (CMongoClient*)userData;
    delete client;
}
/* $end */

int mongodbMain(int argc, char* argv[]) {
/* $begin(main) */
    MONGOCLIENT_HANDLE = corto_olsKey(CMongoClientClenup);

    return 0;
/* $end */
}
