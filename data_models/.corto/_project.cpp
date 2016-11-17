/* _project.cpp
 *
 * This file is generated. Do not modify.
 */

#include <recorto/data_models/data_models.h>
#include <recorto/data_models/_project.h>

int data_modelsMain(int argc, char* argv[]);

int data_models_load(void);

#ifdef __cplusplus
extern "C"
#endif
RECORTO_DATA_MODELS_EXPORT int cortomain(int argc, char* argv[]) {
    if (corto_load("/recorto", 0, NULL)) return -1;
    if (data_models_load()) return -1;
    if (data_modelsMain(argc, argv)) return -1;
    return 0;
}

