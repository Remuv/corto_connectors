/* _project.h
 *
 * This file contains generated code. Do not modify!
 */

#if BUILDING_RECORTO_CLMDB && defined _MSC_VER
#define RECORTO_CLMDB_EXPORT __declspec(dllexport)
#elif BUILDING_RECORTO_CLMDB
#define RECORTO_CLMDB_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define RECORTO_CLMDB_EXPORT __declspec(dllimport)
#else
#define RECORTO_CLMDB_EXPORT
#endif

