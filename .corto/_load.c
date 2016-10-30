/* _load.c
 *
 * This file is generated. Do not modify.
 */

#include <remuv/remuv.h>

int remuv_load(void);
int remuvMain(int argc, char* argv[]);

#ifdef __cplusplus
extern "C"
#endif
REMUV_EXPORT int cortomain(int argc, char* argv[]) {
    if (remuv_load()) return -1;
    if (remuvMain(argc, argv)) return -1;
    return 0;
}

