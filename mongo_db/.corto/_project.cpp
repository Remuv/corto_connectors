/* _project.cpp
 *
 * This file is generated. Do not modify.
 */

#include <recorto/mongodb/mongodb.h>
#include <recorto/mongodb/_project.h>

int mongodbMain(int argc, char* argv[]);

int mongodb_load(void);

#ifdef __cplusplus
extern "C"
#endif
RECORTO_MONGODB_EXPORT int cortomain(int argc, char* argv[]) {
    if (corto_load("/corto/fmt/json", 0, NULL)) return -1;
    if (corto_load("/recorto", 0, NULL)) return -1;
    if (corto_load("/recorto/mongo/driver/mongocxx", 0, NULL)) return -1;
    if (mongodb_load()) return -1;
    if (mongodbMain(argc, argv)) return -1;
    return 0;
}

