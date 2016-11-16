/* _project.cpp
 *
 * This file is generated. Do not modify.
 */

#include <recorto/clmdb/clmdb.h>
#include <recorto/clmdb/_project.h>

int clmdbMain(int argc, char* argv[]);

int clmdb_load(void);

#ifdef __cplusplus
extern "C"
#endif
RECORTO_CLMDB_EXPORT int cortomain(int argc, char* argv[]) {
    if (corto_load("/corto/fmt/json", 0, NULL)) return -1;
    if (corto_load("/recorto", 0, NULL)) return -1;
    if (clmdb_load()) return -1;
    if (clmdbMain(argc, argv)) return -1;
    return 0;
}

