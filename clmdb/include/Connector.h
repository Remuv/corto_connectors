/* Connector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef REMUV_CLMDB_CONNECTOR_H
#define REMUV_CLMDB_CONNECTOR_H

#include <corto/corto.h>
#include <remuv/clmdb/_interface.h>
#include <remuv/clmdb/_type.h>
#include <remuv/clmdb/_api.h>
#include <remuv/clmdb/_meta.h>

#ifdef __cplusplus
extern "C" {
#endif


REMUV_CLMDB_EXPORT corto_int16 _clmdb_Connector_construct(
    clmdb_Connector _this);
#define clmdb_Connector_construct(_this) _clmdb_Connector_construct(clmdb_Connector(_this))

REMUV_CLMDB_EXPORT corto_void _clmdb_Connector_destruct(
    clmdb_Connector _this);
#define clmdb_Connector_destruct(_this) _clmdb_Connector_destruct(clmdb_Connector(_this))

REMUV_CLMDB_EXPORT corto_void _clmdb_Connector_onNotify(
    clmdb_Connector _this,
    corto_eventMask event,
    corto_result *object);
#define clmdb_Connector_onNotify(_this, event, object) _clmdb_Connector_onNotify(clmdb_Connector(_this), event, object)

REMUV_CLMDB_EXPORT corto_resultIter _clmdb_Connector_onRequest(
    clmdb_Connector _this,
    corto_request *request);
#define clmdb_Connector_onRequest(_this, request) _clmdb_Connector_onRequest(clmdb_Connector(_this), request)

#ifdef __cplusplus
}
#endif
#endif

