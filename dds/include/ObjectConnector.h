/* ObjectConnector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef RECORTO_DDS_OBJECTCONNECTOR_H
#define RECORTO_DDS_OBJECTCONNECTOR_H

#include <corto/corto.h>
#include <corto/corto.h>
#include <recorto/dds/_project.h>

#include <recorto/dds/_type.h>
#include <recorto/dds/_load.h>
#include <recorto/dds/_api.h>

#ifdef __cplusplus
extern "C" {
#endif

#define dds_ObjectConnector_construct(_this) _dds_ObjectConnector_construct(dds_ObjectConnector(_this))
RECORTO_DDS_EXPORT
corto_int16 _dds_ObjectConnector_construct(
    dds_ObjectConnector _this);

#define dds_ObjectConnector_destruct(_this) _dds_ObjectConnector_destruct(dds_ObjectConnector(_this))
RECORTO_DDS_EXPORT
corto_void _dds_ObjectConnector_destruct(
    dds_ObjectConnector _this);

#define dds_ObjectConnector_onNotify(_this, event, object) _dds_ObjectConnector_onNotify(dds_ObjectConnector(_this), event, object)
RECORTO_DDS_EXPORT
corto_void _dds_ObjectConnector_onNotify(
    dds_ObjectConnector _this,
    corto_eventMask event,
    corto_result *object);

#define dds_ObjectConnector_onRequest(_this, request) _dds_ObjectConnector_onRequest(dds_ObjectConnector(_this), request)
RECORTO_DDS_EXPORT
corto_resultIter _dds_ObjectConnector_onRequest(
    dds_ObjectConnector _this,
    corto_request *request);

#define dds_ObjectConnector_onSubscribe(_this, parent, expr, ctx) _dds_ObjectConnector_onSubscribe(dds_ObjectConnector(_this), parent, expr, ctx)
RECORTO_DDS_EXPORT
corto_word _dds_ObjectConnector_onSubscribe(
    dds_ObjectConnector _this,
    corto_string parent,
    corto_string expr,
    corto_word ctx);

#define dds_ObjectConnector_onUnsubscribe(_this, parent, expr, ctx) _dds_ObjectConnector_onUnsubscribe(dds_ObjectConnector(_this), parent, expr, ctx)
RECORTO_DDS_EXPORT
corto_word _dds_ObjectConnector_onUnsubscribe(
    dds_ObjectConnector _this,
    corto_string parent,
    corto_string expr,
    corto_word ctx);

#ifdef __cplusplus
}
#endif

#endif

