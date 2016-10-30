/* _meta.c
 *
 * Loads objects in object store.
 * This file contains generated code. Do not modify!
 */

#include <remuv/remuv.h>
/* Variable definitions */
corto_package remuv_o;

/* Load objects in object store. */
int remuv_load(void) {
    corto_object _a_; /* Used for resolving anonymous objects */
    _a_ = NULL;

    corto_attr prevAttr = corto_setAttr(CORTO_ATTR_PERSISTENT);

    remuv_o = corto_package(corto_declareChild(root_o, "remuv", corto_package_o));
    if (!remuv_o) {
        corto_error("remuv_load: failed to declare 'remuv_o' (%s)", corto_lasterr());
        goto error;
    }

    if (!corto_checkState(remuv_o, CORTO_DEFINED)) {
        remuv_o->url = NULL;
        remuv_o->version = NULL;
        remuv_o->author = NULL;
        remuv_o->description = NULL;
        remuv_o->env = NULL;
        remuv_o->dependencies = corto_llNew();
        remuv_o->prefix = NULL;
        remuv_o->cortoVersion = corto_llNew();
        remuv_o->local = FALSE;
        remuv_o->lib = corto_llNew();
        remuv_o->libpath = corto_llNew();
        remuv_o->include = corto_llNew();
        remuv_o->link = corto_llNew();
        if (corto_define(remuv_o)) {
            corto_error("remuv_load: failed to define 'remuv_o' (%s)", corto_lasterr());
            goto error;
        }
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
