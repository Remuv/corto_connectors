/* _meta.cpp
 *
 * Loads objects in object store.
 * This file contains generated code. Do not modify!
 */

#include <remuv/mongo/mongo.h>
/* Variable definitions */
corto_package remuv_mongo_o;
corto_class mongo_Connector_o;
corto_method mongo_Connector_construct_o;
corto_member mongo_Connector_dbname_o;
corto_member mongo_Connector_hostaddr_o;
corto_alias mongo_Connector_mask_o;
corto_alias mongo_Connector_mount_o;
corto_method mongo_Connector_onNotify_o;
corto_method mongo_Connector_onRequest_o;
corto_member mongo_Connector_password_o;
corto_member mongo_Connector_port_o;
corto_member mongo_Connector_user_o;

/* Load objects in object store. */
int mongo_load(void) {
    corto_object _a_; /* Used for resolving anonymous objects */
    _a_ = NULL;

    corto_attr prevAttr = corto_setAttr(CORTO_ATTR_PERSISTENT);

    remuv_mongo_o = corto_package(corto_declareChild(remuv_o, "mongo", corto_package_o));
    if (!remuv_mongo_o) {
        corto_error("mongo_load: failed to declare 'remuv_mongo_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(remuv_mongo_o, CORTO_DEFINED)) {
        remuv_mongo_o->url = NULL;
        remuv_mongo_o->version = NULL;
        remuv_mongo_o->author = NULL;
        remuv_mongo_o->description = NULL;
        remuv_mongo_o->env = NULL;
        remuv_mongo_o->dependencies = corto_llNew();
        remuv_mongo_o->prefix = NULL;
        remuv_mongo_o->cortoVersion = corto_llNew();
        remuv_mongo_o->local = FALSE;
        remuv_mongo_o->lib = corto_llNew();
        remuv_mongo_o->libpath = corto_llNew();
        remuv_mongo_o->include = corto_llNew();
        remuv_mongo_o->link = corto_llNew();
        if (corto_define(remuv_mongo_o)) {
            corto_error("mongo_load: failed to define 'remuv_mongo_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_o = corto_class(corto_declareChild(remuv_mongo_o, "Connector", corto_class_o));
    if (!mongo_Connector_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_o' (%s)", corto_lasterr());
        goto error;
    }

    mongo_Connector_construct_o = corto_method(corto_declareChild(mongo_Connector_o, "construct()", corto_method_o));
    if (!mongo_Connector_construct_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_construct_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_construct_o, CORTO_DEFINED)) {
        ((corto_function)mongo_Connector_construct_o)->returnType = corto_type(corto_resolve(NULL, "int16"));
        ((corto_function)mongo_Connector_construct_o)->returnsReference = FALSE;
        mongo_Connector_construct_o->_virtual = FALSE;
        
        corto_function(mongo_Connector_construct_o)->kind = CORTO_PROCEDURE_CDECL;
        corto_int16 _mongo_Connector_construct(
    mongo_Connector _this);
        corto_function(mongo_Connector_construct_o)->fptr = (corto_word)_mongo_Connector_construct;
        if (corto_define(mongo_Connector_construct_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_construct_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_dbname_o = corto_member(corto_declareChild(mongo_Connector_o, "dbname", corto_member_o));
    if (!mongo_Connector_dbname_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_dbname_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_dbname_o, CORTO_DEFINED)) {
        mongo_Connector_dbname_o->type = corto_type(corto_resolve(NULL, "string"));
        mongo_Connector_dbname_o->modifiers = 0x0;
        mongo_Connector_dbname_o->state = 0x6;
        mongo_Connector_dbname_o->weak = FALSE;
        mongo_Connector_dbname_o->id = 2;
        if (corto_define(mongo_Connector_dbname_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_dbname_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_hostaddr_o = corto_member(corto_declareChild(mongo_Connector_o, "hostaddr", corto_member_o));
    if (!mongo_Connector_hostaddr_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_hostaddr_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_hostaddr_o, CORTO_DEFINED)) {
        mongo_Connector_hostaddr_o->type = corto_type(corto_resolve(NULL, "string"));
        mongo_Connector_hostaddr_o->modifiers = 0x0;
        mongo_Connector_hostaddr_o->state = 0x6;
        mongo_Connector_hostaddr_o->weak = FALSE;
        mongo_Connector_hostaddr_o->id = 5;
        if (corto_define(mongo_Connector_hostaddr_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_hostaddr_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_mask_o = corto_alias(corto_declareChild(mongo_Connector_o, "mask", corto_alias_o));
    if (!mongo_Connector_mask_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_mask_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_mask_o, CORTO_DEFINED)) {
        ((corto_member)mongo_Connector_mask_o)->type = corto_type(corto_resolve(NULL, "/corto/core/eventMask"));
        ((corto_member)mongo_Connector_mask_o)->modifiers = 0x0;
        ((corto_member)mongo_Connector_mask_o)->state = 0x6;
        ((corto_member)mongo_Connector_mask_o)->weak = FALSE;
        ((corto_member)mongo_Connector_mask_o)->id = 1;
        mongo_Connector_mask_o->member = corto_member(corto_resolve(NULL, "/corto/core/mount/mask"));
        if (corto_define(mongo_Connector_mask_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_mask_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_mount_o = corto_alias(corto_declareChild(mongo_Connector_o, "mount", corto_alias_o));
    if (!mongo_Connector_mount_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_mount_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_mount_o, CORTO_DEFINED)) {
        ((corto_member)mongo_Connector_mount_o)->type = corto_type(corto_resolve(NULL, "object"));
        ((corto_member)mongo_Connector_mount_o)->modifiers = 0x0;
        ((corto_member)mongo_Connector_mount_o)->state = 0x6;
        ((corto_member)mongo_Connector_mount_o)->weak = FALSE;
        ((corto_member)mongo_Connector_mount_o)->id = 0;
        mongo_Connector_mount_o->member = corto_member(corto_resolve(NULL, "/corto/core/mount/mount"));
        if (corto_define(mongo_Connector_mount_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_mount_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_onNotify_o = corto_method(corto_declareChild(mongo_Connector_o, "onNotify(/corto/core/eventMask event,/corto/core/result object)", corto_method_o));
    if (!mongo_Connector_onNotify_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_onNotify_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_onNotify_o, CORTO_DEFINED)) {
        ((corto_function)mongo_Connector_onNotify_o)->returnType = corto_type(corto_resolve(NULL, "void"));
        ((corto_function)mongo_Connector_onNotify_o)->returnsReference = FALSE;
        mongo_Connector_onNotify_o->_virtual = FALSE;
        
        corto_function(mongo_Connector_onNotify_o)->kind = CORTO_PROCEDURE_CDECL;
        corto_void _mongo_Connector_onNotify(
    mongo_Connector _this,
    corto_eventMask event,
    corto_result *object);
        corto_function(mongo_Connector_onNotify_o)->fptr = (corto_word)_mongo_Connector_onNotify;
        if (corto_define(mongo_Connector_onNotify_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_onNotify_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_onRequest_o = corto_method(corto_declareChild(mongo_Connector_o, "onRequest(/corto/core/request request)", corto_method_o));
    if (!mongo_Connector_onRequest_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_onRequest_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_onRequest_o, CORTO_DEFINED)) {
        ((corto_function)mongo_Connector_onRequest_o)->returnType = corto_type(corto_resolve(NULL, "/corto/core/resultIter"));
        ((corto_function)mongo_Connector_onRequest_o)->returnsReference = FALSE;
        mongo_Connector_onRequest_o->_virtual = FALSE;
        
        corto_function(mongo_Connector_onRequest_o)->kind = CORTO_PROCEDURE_CDECL;
        corto_resultIter _mongo_Connector_onRequest(
    mongo_Connector _this,
    corto_request *request);
        corto_function(mongo_Connector_onRequest_o)->fptr = (corto_word)_mongo_Connector_onRequest;
        if (corto_define(mongo_Connector_onRequest_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_onRequest_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_password_o = corto_member(corto_declareChild(mongo_Connector_o, "password", corto_member_o));
    if (!mongo_Connector_password_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_password_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_password_o, CORTO_DEFINED)) {
        mongo_Connector_password_o->type = corto_type(corto_resolve(NULL, "string"));
        mongo_Connector_password_o->modifiers = 0x0;
        mongo_Connector_password_o->state = 0x6;
        mongo_Connector_password_o->weak = FALSE;
        mongo_Connector_password_o->id = 4;
        if (corto_define(mongo_Connector_password_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_password_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_port_o = corto_member(corto_declareChild(mongo_Connector_o, "port", corto_member_o));
    if (!mongo_Connector_port_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_port_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_port_o, CORTO_DEFINED)) {
        mongo_Connector_port_o->type = corto_type(corto_resolve(NULL, "int32"));
        mongo_Connector_port_o->modifiers = 0x0;
        mongo_Connector_port_o->state = 0x6;
        mongo_Connector_port_o->weak = FALSE;
        mongo_Connector_port_o->id = 6;
        if (corto_define(mongo_Connector_port_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_port_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    mongo_Connector_user_o = corto_member(corto_declareChild(mongo_Connector_o, "user", corto_member_o));
    if (!mongo_Connector_user_o) {
        corto_error("mongo_load: failed to declare 'mongo_Connector_user_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(mongo_Connector_user_o, CORTO_DEFINED)) {
        mongo_Connector_user_o->type = corto_type(corto_resolve(NULL, "string"));
        mongo_Connector_user_o->modifiers = 0x0;
        mongo_Connector_user_o->state = 0x6;
        mongo_Connector_user_o->weak = FALSE;
        mongo_Connector_user_o->id = 3;
        if (corto_define(mongo_Connector_user_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_user_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    if (!corto_checkState(mongo_Connector_o, CORTO_DEFINED)) {
        ((corto_type)mongo_Connector_o)->kind = CORTO_COMPOSITE;
        ((corto_type)mongo_Connector_o)->reference = TRUE;
        ((corto_type)mongo_Connector_o)->parentType = NULL;
        ((corto_type)mongo_Connector_o)->parentState = 0x6;
        ((corto_type)mongo_Connector_o)->defaultType = NULL;
        ((corto_type)mongo_Connector_o)->defaultProcedureType = NULL;
        ((corto_interface)mongo_Connector_o)->base = corto_interface(corto_resolve(NULL, "/corto/core/mount"));
        ((corto_struct)mongo_Connector_o)->baseAccess = 0x10;
        mongo_Connector_o->implements.length = 0;
        mongo_Connector_o->implements.buffer = NULL;
        if (corto_define(mongo_Connector_o)) {
            corto_error("mongo_load: failed to define 'mongo_Connector_o' (%s)", corto_lasterr());
            goto error;
        }
    }

    if (corto_type(mongo_Connector_o)->size != sizeof(struct mongo_Connector_s)) {
        corto_error("mongo_load: calculated size '%d' of type 'mongo_Connector_o' doesn't match C-type size '%d'", corto_type(mongo_Connector_o)->size, sizeof(struct mongo_Connector_s));
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
