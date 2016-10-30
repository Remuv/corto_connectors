/* _load.c
 *
 * This file is generated. Do not modify.
 */

#include <test.h>

int test_load(void);
int testMain(int argc, char* argv[]);

#ifdef __cplusplus
extern "C"
#endif
TEST_EXPORT int cortomain(int argc, char* argv[]) {
    if (corto_load("/corto/test", 0, NULL)) return -1;
    if (corto_load("/remuv/mongo", 0, NULL)) return -1;
    if (test_load()) return -1;
    if (testMain(argc, argv)) return -1;
    return 0;
}

