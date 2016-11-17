/* _project.c
 *
 * This file is generated. Do not modify.
 */

#include <recorto/recorto.h>
#include <recorto/_project.h>

int recortoMain(int argc, char* argv[]);

int recorto_load(void);

#ifdef __cplusplus
extern "C"
#endif
RECORTO_EXPORT int cortomain(int argc, char* argv[]) {
    if (recorto_load()) return -1;
    if (recortoMain(argc, argv)) return -1;
    return 0;
}

