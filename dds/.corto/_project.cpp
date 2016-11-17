/* _project.cpp
 *
 * This file is generated. Do not modify.
 */

#include <recorto/dds/dds.h>
#include <recorto/dds/_project.h>

int ddsMain(int argc, char* argv[]);

int dds_load(void);

#ifdef __cplusplus
extern "C"
#endif
RECORTO_DDS_EXPORT int cortomain(int argc, char* argv[]) {
    if (corto_load("/common/base", 0, NULL)) return -1;
    if (corto_load("/common/interface/corto", 0, NULL)) return -1;
    if (corto_load("/recorto", 0, NULL)) return -1;
    if (corto_load("/common/dds/vendor", 0, NULL)) return -1;
    if (dds_load()) return -1;
    if (ddsMain(argc, argv)) return -1;
    return 0;
}

