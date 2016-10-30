/* _interface.h
 *
 * This file contains generated code. Do not modify!
 */

#if BUILDING_REMUV && defined _MSC_VER
#define REMUV_DLL_EXPORTED __declspec(dllexport)
#elif BUILDING_REMUV
#define REMUV_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define REMUV_EXPORT __declspec(dllimport)
#else
#define REMUV_EXPORT
#endif

