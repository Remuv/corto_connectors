/* _project.cpp
 *
 * This file is generated. Do not modify.
 */

#include <test.h>
#include <_project.h>

int testMain(int argc, char* argv[]);

int test_load(void);

#ifdef __cplusplus
extern "C"
#endif
TEST_EXPORT int cortomain(int argc, char* argv[]) {
    if (corto_load("/corto/test", 0, NULL)) return -1;
    if (corto_load("/corto/fmt/json", 0, NULL)) return -1;
    if (corto_load("/remuv/clmdb", 0, NULL)) return -1;
    if (corto_load("/corto/admin", 0, NULL)) return -1;
    if (test_load()) return -1;
    if (testMain(argc, argv)) return -1;
    return 0;
}

