/* Connector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef REMUV_MONGO_CONNECTOR_H
#define REMUV_MONGO_CONNECTOR_H

#include <corto/corto.h>
#include <remuv/mongo/_interface.h>
#include <remuv/mongo/_type.h>
#include <remuv/mongo/_api.h>
#include <remuv/mongo/_meta.h>

#ifdef __cplusplus
extern "C" {
#endif


REMUV_MONGO_EXPORT corto_int16 _mongo_Connector_construct(
    mongo_Connector _this);
#define mongo_Connector_construct(_this) _mongo_Connector_construct(mongo_Connector(_this))

REMUV_MONGO_EXPORT corto_void _mongo_Connector_onNotify(
    mongo_Connector _this,
    corto_eventMask event,
    corto_result *object);
#define mongo_Connector_onNotify(_this, event, object) _mongo_Connector_onNotify(mongo_Connector(_this), event, object)

REMUV_MONGO_EXPORT corto_resultIter _mongo_Connector_onRequest(
    mongo_Connector _this,
    corto_request *request);
#define mongo_Connector_onRequest(_this, request) _mongo_Connector_onRequest(mongo_Connector(_this), request)

#ifdef __cplusplus
}
#endif
#endif

