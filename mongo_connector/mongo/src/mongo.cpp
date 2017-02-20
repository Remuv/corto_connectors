/* $CORTO_GENERATED
 *
 * mongo.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/mongo_connector/mongo/mongo.h>

/* $header() */
#include "mongo_util.h"

corto_uint8 MONGOPOOL_HANDLE;

static void CMongoPoolClenup(void *userData) {
    CMongoPool *client = (CMongoPool*)userData;
    delete client;
}
/* $end */

int mongoMain(int argc, char *argv[]) {
/* $begin(main) */
    MONGOPOOL_HANDLE = corto_olsKey(CMongoPoolClenup);
    return 0;
/* $end */
}
