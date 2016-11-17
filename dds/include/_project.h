/* _project.h
 *
 * This file contains generated code. Do not modify!
 */

#if BUILDING_RECORTO_DDS && defined _MSC_VER
#define RECORTO_DDS_EXPORT __declspec(dllexport)
#elif BUILDING_RECORTO_DDS
#define RECORTO_DDS_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define RECORTO_DDS_EXPORT __declspec(dllimport)
#else
#define RECORTO_DDS_EXPORT
#endif

