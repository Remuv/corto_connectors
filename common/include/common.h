/* common.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef RECORTO_COMMON_H
#define RECORTO_COMMON_H

#include <corto/corto.h>
#include <corto/corto.h>
#include <recorto/common/_project.h>
#include <recorto/recorto.h>
#include <corto/core/c/c.h>
#include <corto/lang/c/c.h>

/* $header() */
/* Enter additional code here. */
/* $end */

#include <recorto/common/_type.h>
#include <recorto/common/_load.h>
#include <recorto/common/_api.h>

/* $body() */
corto_uint8 OLS_COPY_CALLBACK_KEY();

typedef void (*CopyCallbackHandler)(corto_object *dst, corto_object src);

#define CORTO_OLS_SET_COPY_CB(type) corto_olsSet(type##_o, OLS_COPY_CALLBACK_KEY(), (void*)type##Copy)
#define CORTO_OLS_GET_COPY_CB(type) (CopyCallbackHandler)corto_olsGet(type, OLS_COPY_CALLBACK_KEY())

/* $end */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif

