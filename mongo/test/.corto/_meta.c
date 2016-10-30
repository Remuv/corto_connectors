/* _meta.c
 *
 * Loads objects in object store.
 * This file contains generated code. Do not modify!
 */

#include <test.h>
/* Variable definitions */
corto_package test_o;
corto_class test_Data_o;
corto_member test_Data_d_o;
corto_member test_Data_f_o;
corto_member test_Data_i8_o;
corto_member test_Data_p_o;
corto_member test_Data_str_o;
test_Suite test_MySuite_o;
test_Case test_MySuite_testSomething_o;
corto_class test_Vec2_o;
corto_member test_Vec2_x_o;
corto_member test_Vec2_y_o;

/* Load objects in object store. */
int test_load(void) {
    corto_object _a_; /* Used for resolving anonymous objects */
    _a_ = NULL;

    corto_attr prevAttr = corto_setAttr(CORTO_ATTR_PERSISTENT);

    test_o = corto_package(corto_declareChild(root_o, "test", corto_package_o));
    if (!test_o) {
        corto_error("test_load: failed to declare 'test_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_o, CORTO_DEFINED)) {
        test_o->url = NULL;
        test_o->version = NULL;
        test_o->author = NULL;
        test_o->description = NULL;
        test_o->env = NULL;
        test_o->dependencies = corto_llNew();
        test_o->prefix = NULL;
        test_o->cortoVersion = corto_llNew();
        test_o->local = FALSE;
        test_o->lib = corto_llNew();
        test_o->libpath = corto_llNew();
        test_o->include = corto_llNew();
        test_o->link = corto_llNew();
        if (corto_define(test_o)) {
            corto_error("test_load: failed to define 'test_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    test_Data_o = corto_class(corto_declareChild(test_o, "Data", corto_class_o));
    if (!test_Data_o) {
        corto_error("test_load: failed to declare 'test_Data_o' (%s)", corto_lasterr());
        goto error;
    }

    test_Data_d_o = corto_member(corto_declareChild(test_Data_o, "d", corto_member_o));
    if (!test_Data_d_o) {
        corto_error("test_load: failed to declare 'test_Data_d_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_Data_d_o, CORTO_DEFINED)) {
        test_Data_d_o->type = corto_type(corto_resolve(NULL, "float64"));
        test_Data_d_o->modifiers = 0x0;
        test_Data_d_o->state = 0x6;
        test_Data_d_o->weak = FALSE;
        test_Data_d_o->id = 3;
        if (corto_define(test_Data_d_o)) {
            corto_error("test_load: failed to define 'test_Data_d_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    test_Data_f_o = corto_member(corto_declareChild(test_Data_o, "f", corto_member_o));
    if (!test_Data_f_o) {
        corto_error("test_load: failed to declare 'test_Data_f_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_Data_f_o, CORTO_DEFINED)) {
        test_Data_f_o->type = corto_type(corto_resolve(NULL, "float32"));
        test_Data_f_o->modifiers = 0x0;
        test_Data_f_o->state = 0x6;
        test_Data_f_o->weak = FALSE;
        test_Data_f_o->id = 2;
        if (corto_define(test_Data_f_o)) {
            corto_error("test_load: failed to define 'test_Data_f_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    test_Data_i8_o = corto_member(corto_declareChild(test_Data_o, "i8", corto_member_o));
    if (!test_Data_i8_o) {
        corto_error("test_load: failed to declare 'test_Data_i8_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_Data_i8_o, CORTO_DEFINED)) {
        test_Data_i8_o->type = corto_type(corto_resolve(NULL, "int8"));
        test_Data_i8_o->modifiers = 0x0;
        test_Data_i8_o->state = 0x6;
        test_Data_i8_o->weak = FALSE;
        test_Data_i8_o->id = 1;
        if (corto_define(test_Data_i8_o)) {
            corto_error("test_load: failed to define 'test_Data_i8_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    test_Data_p_o = corto_member(corto_declareChild(test_Data_o, "p", corto_member_o));
    if (!test_Data_p_o) {
        corto_error("test_load: failed to declare 'test_Data_p_o' (%s)", corto_lasterr());
        goto error;
    }

    test_Data_str_o = corto_member(corto_declareChild(test_Data_o, "str", corto_member_o));
    if (!test_Data_str_o) {
        corto_error("test_load: failed to declare 'test_Data_str_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_Data_str_o, CORTO_DEFINED)) {
        test_Data_str_o->type = corto_type(corto_resolve(NULL, "string"));
        test_Data_str_o->modifiers = 0x0;
        test_Data_str_o->state = 0x6;
        test_Data_str_o->weak = FALSE;
        test_Data_str_o->id = 0;
        if (corto_define(test_Data_str_o)) {
            corto_error("test_load: failed to define 'test_Data_str_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    test_MySuite_o = test_Suite(corto_declareChild(test_o, "MySuite", test_Suite_o));
    if (!test_MySuite_o) {
        corto_error("test_load: failed to declare 'test_MySuite_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_MySuite_o, CORTO_DEFINED)) {
        ((corto_type)test_MySuite_o)->kind = CORTO_COMPOSITE;
        ((corto_type)test_MySuite_o)->reference = TRUE;
        ((corto_type)test_MySuite_o)->parentType = NULL;
        ((corto_type)test_MySuite_o)->parentState = 0x6;
        ((corto_type)test_MySuite_o)->defaultType = NULL;
        ((corto_type)test_MySuite_o)->defaultProcedureType = NULL;
        ((corto_interface)test_MySuite_o)->base = corto_interface(corto_resolve(NULL, "/corto/test/SuiteData"));
        ((corto_struct)test_MySuite_o)->baseAccess = 0x0;
        ((corto_class)test_MySuite_o)->implements.length = 0;
        ((corto_class)test_MySuite_o)->implements.buffer = NULL;
        if (corto_define(test_MySuite_o)) {
            corto_error("test_load: failed to define 'test_MySuite_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    if (corto_type(test_MySuite_o)->size != sizeof(struct test_MySuite_s)) {
        corto_error("test_load: calculated size '%d' of type 'test_MySuite_o' doesn't match C-type size '%d'", corto_type(test_MySuite_o)->size, sizeof(struct test_MySuite_s));
    }

    test_MySuite_testSomething_o = test_Case(corto_declareChild(test_MySuite_o, "testSomething()", test_Case_o));
    if (!test_MySuite_testSomething_o) {
        corto_error("test_load: failed to declare 'test_MySuite_testSomething_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_MySuite_testSomething_o, CORTO_DEFINED)) {
        ((corto_function)test_MySuite_testSomething_o)->returnType = corto_type(corto_resolve(NULL, "void"));
        ((corto_function)test_MySuite_testSomething_o)->returnsReference = FALSE;
        ((corto_method)test_MySuite_testSomething_o)->_virtual = FALSE;
        
        corto_function(test_MySuite_testSomething_o)->kind = CORTO_PROCEDURE_CDECL;
        corto_void _test_MySuite_testSomething(
    test_MySuite _this);
        corto_function(test_MySuite_testSomething_o)->fptr = (corto_word)_test_MySuite_testSomething;
        if (corto_define(test_MySuite_testSomething_o)) {
            corto_error("test_load: failed to define 'test_MySuite_testSomething_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    test_Vec2_o = corto_class(corto_declareChild(test_o, "Vec2", corto_class_o));
    if (!test_Vec2_o) {
        corto_error("test_load: failed to declare 'test_Vec2_o' (%s)", corto_lasterr());
        goto error;
    }

    test_Vec2_x_o = corto_member(corto_declareChild(test_Vec2_o, "x", corto_member_o));
    if (!test_Vec2_x_o) {
        corto_error("test_load: failed to declare 'test_Vec2_x_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_Vec2_x_o, CORTO_DEFINED)) {
        test_Vec2_x_o->type = corto_type(corto_resolve(NULL, "float32"));
        test_Vec2_x_o->modifiers = 0x0;
        test_Vec2_x_o->state = 0x6;
        test_Vec2_x_o->weak = FALSE;
        test_Vec2_x_o->id = 0;
        if (corto_define(test_Vec2_x_o)) {
            corto_error("test_load: failed to define 'test_Vec2_x_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    test_Vec2_y_o = corto_member(corto_declareChild(test_Vec2_o, "y", corto_member_o));
    if (!test_Vec2_y_o) {
        corto_error("test_load: failed to declare 'test_Vec2_y_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(test_Vec2_y_o, CORTO_DEFINED)) {
        test_Vec2_y_o->type = corto_type(corto_resolve(NULL, "float32"));
        test_Vec2_y_o->modifiers = 0x0;
        test_Vec2_y_o->state = 0x6;
        test_Vec2_y_o->weak = FALSE;
        test_Vec2_y_o->id = 1;
        if (corto_define(test_Vec2_y_o)) {
            corto_error("test_load: failed to define 'test_Vec2_y_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    if (!corto_checkState(test_Vec2_o, CORTO_DEFINED)) {
        ((corto_type)test_Vec2_o)->kind = CORTO_COMPOSITE;
        ((corto_type)test_Vec2_o)->reference = TRUE;
        ((corto_type)test_Vec2_o)->parentType = NULL;
        ((corto_type)test_Vec2_o)->parentState = 0x6;
        ((corto_type)test_Vec2_o)->defaultType = NULL;
        ((corto_type)test_Vec2_o)->defaultProcedureType = NULL;
        ((corto_interface)test_Vec2_o)->base = NULL;
        ((corto_struct)test_Vec2_o)->baseAccess = 0x0;
        test_Vec2_o->implements.length = 0;
        test_Vec2_o->implements.buffer = NULL;
        if (corto_define(test_Vec2_o)) {
            corto_error("test_load: failed to define 'test_Vec2_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    if (corto_type(test_Vec2_o)->size != sizeof(struct test_Vec2_s)) {
        corto_error("test_load: calculated size '%d' of type 'test_Vec2_o' doesn't match C-type size '%d'", corto_type(test_Vec2_o)->size, sizeof(struct test_Vec2_s));
    }

    if (!corto_checkState(test_Data_p_o, CORTO_DEFINED)) {
        test_Data_p_o->type = corto_type(corto_resolve(NULL, "/test/Vec2"));
        test_Data_p_o->modifiers = 0x0;
        test_Data_p_o->state = 0x6;
        test_Data_p_o->weak = FALSE;
        test_Data_p_o->id = 4;
        if (corto_define(test_Data_p_o)) {
            corto_error("test_load: failed to define 'test_Data_p_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    if (!corto_checkState(test_Data_o, CORTO_DEFINED)) {
        ((corto_type)test_Data_o)->kind = CORTO_COMPOSITE;
        ((corto_type)test_Data_o)->reference = TRUE;
        ((corto_type)test_Data_o)->parentType = NULL;
        ((corto_type)test_Data_o)->parentState = 0x6;
        ((corto_type)test_Data_o)->defaultType = NULL;
        ((corto_type)test_Data_o)->defaultProcedureType = NULL;
        ((corto_interface)test_Data_o)->base = NULL;
        ((corto_struct)test_Data_o)->baseAccess = 0x0;
        test_Data_o->implements.length = 0;
        test_Data_o->implements.buffer = NULL;
        if (corto_define(test_Data_o)) {
            corto_error("test_load: failed to define 'test_Data_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    if (corto_type(test_Data_o)->size != sizeof(struct test_Data_s)) {
        corto_error("test_load: calculated size '%d' of type 'test_Data_o' doesn't match C-type size '%d'", corto_type(test_Data_o)->size, sizeof(struct test_Data_s));
    }

    corto_setAttr(prevAttr);

    if (_a_) {
        corto_release(_a_);
    }

    return 0;
error:
    if (_a_) {
        corto_release(_a_);
    }

    return -1;
}
