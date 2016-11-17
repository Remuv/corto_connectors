/* _project.h
 *
 * This file contains generated code. Do not modify!
 */

#if BUILDING_RECORTO && defined _MSC_VER
#define RECORTO_EXPORT __declspec(dllexport)
#elif BUILDING_RECORTO
#define RECORTO_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define RECORTO_EXPORT __declspec(dllimport)
#else
#define RECORTO_EXPORT
#endif

