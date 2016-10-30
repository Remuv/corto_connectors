/* _api.c
 *
 * API convenience functions for C-language.
 * This file contains generated code. Do not modify!
 */

#include <test.h>
test_Data _test_DataCreate(corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p) {
    test_Data _this;
    _this = test_Data(corto_declare(test_Data_o));
    if (!_this) {
        return NULL;
    }
    if (!corto_checkState(_this, CORTO_DEFINED)) {
        corto_setstr(&((test_Data)_this)->str, str);
        ((test_Data)_this)->i8 = i8;
        ((test_Data)_this)->f = f;
        ((test_Data)_this)->d = d;
        corto_setref(&((test_Data)_this)->p, p);
        if (corto_define(_this)) {
            corto_release(_this);
            _this = NULL;
        }
    }
    return _this;
}

test_Data _test_DataCreateChild(corto_object _parent, corto_string _name, corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p) {
    test_Data _this;
    _this = test_Data(corto_declareChild(_parent, _name, test_Data_o));
    if (!_this) {
        return NULL;
    }
    if (!corto_checkState(_this, CORTO_DEFINED)) {
        corto_setstr(&((test_Data)_this)->str, str);
        ((test_Data)_this)->i8 = i8;
        ((test_Data)_this)->f = f;
        ((test_Data)_this)->d = d;
        corto_setref(&((test_Data)_this)->p, p);
        if (corto_define(_this)) {
            corto_release(_this);
            _this = NULL;
        }
    }
    return _this;
}

corto_int16 _test_DataUpdate(test_Data _this, corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p) {
    CORTO_UNUSED(_this);
    if (!corto_updateBegin(_this)) {
        corto_setstr(&((test_Data)_this)->str, str);
        ((test_Data)_this)->i8 = i8;
        ((test_Data)_this)->f = f;
        ((test_Data)_this)->d = d;
        corto_setref(&((test_Data)_this)->p, p);
        corto_updateEnd(_this);
    } else {
        return -1;
    }
    return 0;
}

test_Data _test_DataDeclare(void) {
    test_Data _this;
    _this = test_Data(corto_declare(test_Data_o));
    if (!_this) {
        return NULL;
    }
    return _this;
}

test_Data _test_DataDeclareChild(corto_object _parent, corto_string _name) {
    test_Data _this;
    _this = test_Data(corto_declareChild(_parent, _name, test_Data_o));
    if (!_this) {
        return NULL;
    }
    return _this;
}

corto_int16 _test_DataDefine(test_Data _this, corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p) {
    CORTO_UNUSED(_this);
    corto_setstr(&((test_Data)_this)->str, str);
    ((test_Data)_this)->i8 = i8;
    ((test_Data)_this)->f = f;
    ((test_Data)_this)->d = d;
    corto_setref(&((test_Data)_this)->p, p);
    return corto_define(_this);
}

test_Data _test_DataAssign(test_Data _this, corto_string str, corto_int8 i8, corto_float32 f, corto_float64 d, test_Vec2 p) {
    CORTO_UNUSED(_this);
    corto_setstr(&((test_Data)_this)->str, str);
    ((test_Data)_this)->i8 = i8;
    ((test_Data)_this)->f = f;
    ((test_Data)_this)->d = d;
    corto_setref(&((test_Data)_this)->p, p);
    return _this;
}

corto_string _test_DataStr(test_Data value) {
    corto_string result;
    corto_value v;
    v = corto_value_object(value, corto_type(test_Data_o));
    result = corto_strv(&v, 0);
    return result;
}

test_Data test_DataFromStr(test_Data value, corto_string str) {
    corto_fromStrp(&value, corto_type(test_Data_o), str);
    return value;
}

corto_equalityKind _test_DataCompare(test_Data dst, test_Data src) {
    return corto_compare(dst, src);
}

test_MySuite _test_MySuiteCreate(corto_uint32 assertCount) {
    test_MySuite _this;
    _this = test_MySuite(corto_declare(test_MySuite_o));
    if (!_this) {
        return NULL;
    }
    if (!corto_checkState(_this, CORTO_DEFINED)) {
        ((test_SuiteData)_this)->assertCount = assertCount;
        if (corto_define(_this)) {
            corto_release(_this);
            _this = NULL;
        }
    }
    return _this;
}

test_MySuite _test_MySuiteCreateChild(corto_object _parent, corto_string _name, corto_uint32 assertCount) {
    test_MySuite _this;
    _this = test_MySuite(corto_declareChild(_parent, _name, test_MySuite_o));
    if (!_this) {
        return NULL;
    }
    if (!corto_checkState(_this, CORTO_DEFINED)) {
        ((test_SuiteData)_this)->assertCount = assertCount;
        if (corto_define(_this)) {
            corto_release(_this);
            _this = NULL;
        }
    }
    return _this;
}

corto_int16 _test_MySuiteUpdate(test_MySuite _this, corto_uint32 assertCount) {
    CORTO_UNUSED(_this);
    if (!corto_updateBegin(_this)) {
        ((test_SuiteData)_this)->assertCount = assertCount;
        corto_updateEnd(_this);
    } else {
        return -1;
    }
    return 0;
}

test_MySuite _test_MySuiteDeclare(void) {
    test_MySuite _this;
    _this = test_MySuite(corto_declare(test_MySuite_o));
    if (!_this) {
        return NULL;
    }
    return _this;
}

test_MySuite _test_MySuiteDeclareChild(corto_object _parent, corto_string _name) {
    test_MySuite _this;
    _this = test_MySuite(corto_declareChild(_parent, _name, test_MySuite_o));
    if (!_this) {
        return NULL;
    }
    return _this;
}

corto_int16 _test_MySuiteDefine(test_MySuite _this, corto_uint32 assertCount) {
    CORTO_UNUSED(_this);
    ((test_SuiteData)_this)->assertCount = assertCount;
    return corto_define(_this);
}

test_MySuite _test_MySuiteAssign(test_MySuite _this, corto_uint32 assertCount) {
    CORTO_UNUSED(_this);
    ((test_SuiteData)_this)->assertCount = assertCount;
    return _this;
}

corto_string _test_MySuiteStr(test_MySuite value) {
    corto_string result;
    corto_value v;
    v = corto_value_object(value, corto_type(test_MySuite_o));
    result = corto_strv(&v, 0);
    return result;
}

test_MySuite test_MySuiteFromStr(test_MySuite value, corto_string str) {
    corto_fromStrp(&value, corto_type(test_MySuite_o), str);
    return value;
}

corto_equalityKind _test_MySuiteCompare(test_MySuite dst, test_MySuite src) {
    return corto_compare(dst, src);
}

test_Vec2 _test_Vec2Create(corto_float32 x, corto_float32 y) {
    test_Vec2 _this;
    _this = test_Vec2(corto_declare(test_Vec2_o));
    if (!_this) {
        return NULL;
    }
    if (!corto_checkState(_this, CORTO_DEFINED)) {
        ((test_Vec2)_this)->x = x;
        ((test_Vec2)_this)->y = y;
        if (corto_define(_this)) {
            corto_release(_this);
            _this = NULL;
        }
    }
    return _this;
}

test_Vec2 _test_Vec2CreateChild(corto_object _parent, corto_string _name, corto_float32 x, corto_float32 y) {
    test_Vec2 _this;
    _this = test_Vec2(corto_declareChild(_parent, _name, test_Vec2_o));
    if (!_this) {
        return NULL;
    }
    if (!corto_checkState(_this, CORTO_DEFINED)) {
        ((test_Vec2)_this)->x = x;
        ((test_Vec2)_this)->y = y;
        if (corto_define(_this)) {
            corto_release(_this);
            _this = NULL;
        }
    }
    return _this;
}

corto_int16 _test_Vec2Update(test_Vec2 _this, corto_float32 x, corto_float32 y) {
    CORTO_UNUSED(_this);
    if (!corto_updateBegin(_this)) {
        ((test_Vec2)_this)->x = x;
        ((test_Vec2)_this)->y = y;
        corto_updateEnd(_this);
    } else {
        return -1;
    }
    return 0;
}

test_Vec2 _test_Vec2Declare(void) {
    test_Vec2 _this;
    _this = test_Vec2(corto_declare(test_Vec2_o));
    if (!_this) {
        return NULL;
    }
    return _this;
}

test_Vec2 _test_Vec2DeclareChild(corto_object _parent, corto_string _name) {
    test_Vec2 _this;
    _this = test_Vec2(corto_declareChild(_parent, _name, test_Vec2_o));
    if (!_this) {
        return NULL;
    }
    return _this;
}

corto_int16 _test_Vec2Define(test_Vec2 _this, corto_float32 x, corto_float32 y) {
    CORTO_UNUSED(_this);
    ((test_Vec2)_this)->x = x;
    ((test_Vec2)_this)->y = y;
    return corto_define(_this);
}

test_Vec2 _test_Vec2Assign(test_Vec2 _this, corto_float32 x, corto_float32 y) {
    CORTO_UNUSED(_this);
    ((test_Vec2)_this)->x = x;
    ((test_Vec2)_this)->y = y;
    return _this;
}

corto_string _test_Vec2Str(test_Vec2 value) {
    corto_string result;
    corto_value v;
    v = corto_value_object(value, corto_type(test_Vec2_o));
    result = corto_strv(&v, 0);
    return result;
}

test_Vec2 test_Vec2FromStr(test_Vec2 value, corto_string str) {
    corto_fromStrp(&value, corto_type(test_Vec2_o), str);
    return value;
}

corto_equalityKind _test_Vec2Compare(test_Vec2 dst, test_Vec2 src) {
    return corto_compare(dst, src);
}

