/* _type.h
 *
 * Type definitions for C-language.
 * This file contains generated code. Do not modify!
 */

#ifndef TEST__TYPE_H
#define TEST__TYPE_H

#include <corto/corto.h>
#include <corto/test/_type.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Casting macro's */
#define test_Vec2(o) ((test_Vec2)corto_assertType((corto_type)test_Vec2_o, o))
#define test_Data(o) ((test_Data)corto_assertType((corto_type)test_Data_o, o))
#define test_MySuite(o) ((test_MySuite)corto_assertType((corto_type)test_MySuite_o, o))

/* Type definitions */
/*  /test/Vec2 */
CORTO_CLASS(test_Vec2);

CORTO_CLASS_DEF(test_Vec2) {
    corto_float32 x;
    corto_float32 y;
};

/*  /test/Data */
CORTO_CLASS(test_Data);

CORTO_CLASS_DEF(test_Data) {
    corto_string str;
    corto_int8 i8;
    corto_float32 f;
    corto_float64 d;
    test_Vec2 p;
};

/*  /test/MySuite */
CORTO_CLASS(test_MySuite);

CORTO_CLASS_DEF(test_MySuite) {
    CORTO_EXTEND(test_SuiteData);
};

#ifdef __cplusplus
}
#endif
#endif

