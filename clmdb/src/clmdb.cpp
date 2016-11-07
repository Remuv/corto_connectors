/* $CORTO_GENERATED
 *
 * clmdb.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/clmdb/clmdb.h>

/* $header() */
corto_threadKey CLMDB_TLS_KEY;

void CLMDB_TLS_CLEAN(void *d)
{
    if (d != nullptr)
    {
        free(d);
    }
}
/* $end */

int clmdbMain(int argc, char* argv[]) {
/* $begin(main) */
    corto_threadTlsKey(&CLMDB_TLS_KEY, CLMDB_TLS_CLEAN);
    return 0;
/* $end */
}
