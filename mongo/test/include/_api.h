/* _api.h
 *
 * API convenience functions for C-language.
 * This file contains generated code. Do not modify!
 */

#ifndef TEST__API_H
#define TEST__API_H

#include <corto/corto.h>
#include <_interface.h>
#ifdef __cplusplus
extern "C" {
#endif
/* /test/Data */
TEST_EXPORT test_Data _test_DataCreate(corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p);
#define test_DataCreate(str, i8, f, d, p) _test_DataCreate(str, i8, f, d, test_Vec2(p))
#define test_DataCreate_auto(_name, str, i8, f, d, p) test_Data _name = test_DataCreate(str, i8, f, d, p); (void)_name
TEST_EXPORT test_Data _test_DataCreateChild(corto_object _parent, corto_string _name, corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p);
#define test_DataCreateChild(_parent, _name, str, i8, f, d, p) _test_DataCreateChild(_parent, _name, str, i8, f, d, test_Vec2(p))
#define test_DataCreateChild_auto(_parent, _name, str, i8, f, d, p) test_Data _name = test_DataCreateChild(_parent, #_name, str, i8, f, d, p); (void)_name
TEST_EXPORT corto_int16 _test_DataUpdate(test_Data _this, corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p);
#define test_DataUpdate(_this, str, i8, f, d, p) _test_DataUpdate(test_Data(_this), str, i8, f, d, test_Vec2(p))

TEST_EXPORT test_Data _test_DataDeclare(void);
#define test_DataDeclare() _test_DataDeclare()
#define test_DataDeclare_auto(_name) test_Data _name = test_DataDeclare(); (void)_name
TEST_EXPORT test_Data _test_DataDeclareChild(corto_object _parent, corto_string _name);
#define test_DataDeclareChild(_parent, _name) _test_DataDeclareChild(_parent, _name)
#define test_DataDeclareChild_auto(_parent, _name) test_Data _name = test_DataDeclareChild(_parent, #_name); (void)_name
TEST_EXPORT corto_int16 _test_DataDefine(test_Data _this, corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p);
#define test_DataDefine(_this, str, i8, f, d, p) _test_DataDefine(test_Data(_this), str, i8, f, d, test_Vec2(p))
TEST_EXPORT test_Data _test_DataAssign(test_Data _this, corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p);
#define test_Data__optional_NotSet NULL
#define test_Data__optional_Set(str, i8, f, d, p) test_DataAssign((test_Data*)corto_calloc(sizeof(test_Data)), str, i8, f, d, p)
#define test_Data__optional_SetCond(cond, str, i8, f, d, p) cond ? test_DataAssign((test_Data*)corto_calloc(sizeof(test_Data)), str, i8, f, d, p) : NULL
#define test_DataUnset(_this) _this ? corto_deinitp(_this, test_Data_o) : 0; corto_dealloc(_this); _this = NULL;
#define test_DataAssign(_this, str, i8, f, d, p) _test_DataAssign(test_Data(_this), str, i8, f, d, test_Vec2(p))
#define test_DataSet(_this, str, i8, f, d, p) _this = _this ? _this : (test_Data*)corto_calloc(sizeof(test_Data)); _test_DataAssign(test_Data(_this), str, i8, f, d, test_Vec2(p))
TEST_EXPORT corto_string _test_DataStr(test_Data value);
#define test_DataStr(value) _test_DataStr(test_Data(value))
TEST_EXPORT test_Data test_DataFromStr(test_Data value, corto_string str);
TEST_EXPORT corto_equalityKind _test_DataCompare(test_Data dst, test_Data src);
#define test_DataCompare(dst, src) _test_DataCompare(test_Data(dst), test_Data(src))

/* /test/MySuite */
TEST_EXPORT test_MySuite _test_MySuiteCreate(corto_uint32 assertCount);
#define test_MySuiteCreate(assertCount) _test_MySuiteCreate(assertCount)
#define test_MySuiteCreate_auto(_name, assertCount) test_MySuite _name = test_MySuiteCreate(assertCount); (void)_name
TEST_EXPORT test_MySuite _test_MySuiteCreateChild(corto_object _parent, corto_string _name, corto_uint32 assertCount);
#define test_MySuiteCreateChild(_parent, _name, assertCount) _test_MySuiteCreateChild(_parent, _name, assertCount)
#define test_MySuiteCreateChild_auto(_parent, _name, assertCount) test_MySuite _name = test_MySuiteCreateChild(_parent, #_name, assertCount); (void)_name
TEST_EXPORT corto_int16 _test_MySuiteUpdate(test_MySuite _this, corto_uint32 assertCount);
#define test_MySuiteUpdate(_this, assertCount) _test_MySuiteUpdate(test_MySuite(_this), assertCount)

TEST_EXPORT test_MySuite _test_MySuiteDeclare(void);
#define test_MySuiteDeclare() _test_MySuiteDeclare()
#define test_MySuiteDeclare_auto(_name) test_MySuite _name = test_MySuiteDeclare(); (void)_name
TEST_EXPORT test_MySuite _test_MySuiteDeclareChild(corto_object _parent, corto_string _name);
#define test_MySuiteDeclareChild(_parent, _name) _test_MySuiteDeclareChild(_parent, _name)
#define test_MySuiteDeclareChild_auto(_parent, _name) test_MySuite _name = test_MySuiteDeclareChild(_parent, #_name); (void)_name
TEST_EXPORT corto_int16 _test_MySuiteDefine(test_MySuite _this, corto_uint32 assertCount);
#define test_MySuiteDefine(_this, assertCount) _test_MySuiteDefine(test_MySuite(_this), assertCount)
TEST_EXPORT test_MySuite _test_MySuiteAssign(test_MySuite _this, corto_uint32 assertCount);
#define test_MySuite__optional_NotSet NULL
#define test_MySuite__optional_Set(assertCount) test_MySuiteAssign((test_MySuite*)corto_calloc(sizeof(test_MySuite)), assertCount)
#define test_MySuite__optional_SetCond(cond, assertCount) cond ? test_MySuiteAssign((test_MySuite*)corto_calloc(sizeof(test_MySuite)), assertCount) : NULL
#define test_MySuiteUnset(_this) _this ? corto_deinitp(_this, test_MySuite_o) : 0; corto_dealloc(_this); _this = NULL;
#define test_MySuiteAssign(_this, assertCount) _test_MySuiteAssign(test_MySuite(_this), assertCount)
#define test_MySuiteSet(_this, assertCount) _this = _this ? _this : (test_MySuite*)corto_calloc(sizeof(test_MySuite)); _test_MySuiteAssign(test_MySuite(_this), assertCount)
TEST_EXPORT corto_string _test_MySuiteStr(test_MySuite value);
#define test_MySuiteStr(value) _test_MySuiteStr(test_MySuite(value))
TEST_EXPORT test_MySuite test_MySuiteFromStr(test_MySuite value, corto_string str);
TEST_EXPORT corto_equalityKind _test_MySuiteCompare(test_MySuite dst, test_MySuite src);
#define test_MySuiteCompare(dst, src) _test_MySuiteCompare(test_MySuite(dst), test_MySuite(src))

/* /test/Vec2 */
TEST_EXPORT test_Vec2 _test_Vec2Create(corto_float32 x, corto_float32 y);
#define test_Vec2Create(x, y) _test_Vec2Create(x, y)
#define test_Vec2Create_auto(_name, x, y) test_Vec2 _name = test_Vec2Create(x, y); (void)_name
TEST_EXPORT test_Vec2 _test_Vec2CreateChild(corto_object _parent, corto_string _name, corto_float32 x, corto_float32 y);
#define test_Vec2CreateChild(_parent, _name, x, y) _test_Vec2CreateChild(_parent, _name, x, y)
#define test_Vec2CreateChild_auto(_parent, _name, x, y) test_Vec2 _name = test_Vec2CreateChild(_parent, #_name, x, y); (void)_name
TEST_EXPORT corto_int16 _test_Vec2Update(test_Vec2 _this, corto_float32 x, corto_float32 y);
#define test_Vec2Update(_this, x, y) _test_Vec2Update(test_Vec2(_this), x, y)

TEST_EXPORT test_Vec2 _test_Vec2Declare(void);
#define test_Vec2Declare() _test_Vec2Declare()
#define test_Vec2Declare_auto(_name) test_Vec2 _name = test_Vec2Declare(); (void)_name
TEST_EXPORT test_Vec2 _test_Vec2DeclareChild(corto_object _parent, corto_string _name);
#define test_Vec2DeclareChild(_parent, _name) _test_Vec2DeclareChild(_parent, _name)
#define test_Vec2DeclareChild_auto(_parent, _name) test_Vec2 _name = test_Vec2DeclareChild(_parent, #_name); (void)_name
TEST_EXPORT corto_int16 _test_Vec2Define(test_Vec2 _this, corto_float32 x, corto_float32 y);
#define test_Vec2Define(_this, x, y) _test_Vec2Define(test_Vec2(_this), x, y)
TEST_EXPORT test_Vec2 _test_Vec2Assign(test_Vec2 _this, corto_float32 x, corto_float32 y);
#define test_Vec2__optional_NotSet NULL
#define test_Vec2__optional_Set(x, y) test_Vec2Assign((test_Vec2*)corto_calloc(sizeof(test_Vec2)), x, y)
#define test_Vec2__optional_SetCond(cond, x, y) cond ? test_Vec2Assign((test_Vec2*)corto_calloc(sizeof(test_Vec2)), x, y) : NULL
#define test_Vec2Unset(_this) _this ? corto_deinitp(_this, test_Vec2_o) : 0; corto_dealloc(_this); _this = NULL;
#define test_Vec2Assign(_this, x, y) _test_Vec2Assign(test_Vec2(_this), x, y)
#define test_Vec2Set(_this, x, y) _this = _this ? _this : (test_Vec2*)corto_calloc(sizeof(test_Vec2)); _test_Vec2Assign(test_Vec2(_this), x, y)
TEST_EXPORT corto_string _test_Vec2Str(test_Vec2 value);
#define test_Vec2Str(value) _test_Vec2Str(test_Vec2(value))
TEST_EXPORT test_Vec2 test_Vec2FromStr(test_Vec2 value, corto_string str);
TEST_EXPORT corto_equalityKind _test_Vec2Compare(test_Vec2 dst, test_Vec2 src);
#define test_Vec2Compare(dst, src) _test_Vec2Compare(test_Vec2(dst), test_Vec2(src))


#ifdef __cplusplus
}
#endif
#endif

