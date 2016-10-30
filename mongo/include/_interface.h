/* _interface.h
 *
 * This file contains generated code. Do not modify!
 */

#if BUILDING_REMUV_MONGO && defined _MSC_VER
#define REMUV/MONGO_DLL_EXPORTED __declspec(dllexport)
#elif BUILDING_REMUV_MONGO
#define REMUV_MONGO_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define REMUV_MONGO_EXPORT __declspec(dllimport)
#else
#define REMUV_MONGO_EXPORT
#endif

