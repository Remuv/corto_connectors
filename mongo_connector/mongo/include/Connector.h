/* Connector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef RECORTO_MONGO_CONNECTOR_MONGO_CONNECTOR_H
#define RECORTO_MONGO_CONNECTOR_MONGO_CONNECTOR_H

#include <corto/corto.h>
#include <recorto/mongo_connector/mongo/_project.h>
#include <recorto/mongo_connector/mongo/_type.h>

#ifdef __cplusplus
extern "C" {
#endif


RECORTO_MONGO_CONNECTOR_MONGO_EXPORT corto_int16 _mongo_Connector_construct(
    mongo_Connector _this);
#define mongo_Connector_construct(_this) _mongo_Connector_construct(mongo_Connector(_this))

RECORTO_MONGO_CONNECTOR_MONGO_EXPORT corto_void _mongo_Connector_onNotify(
    mongo_Connector _this,
    corto_eventMask event,
    corto_result *object);
#define mongo_Connector_onNotify(_this, event, object) _mongo_Connector_onNotify(mongo_Connector(_this), event, object)

RECORTO_MONGO_CONNECTOR_MONGO_EXPORT corto_resultIter _mongo_Connector_onRequest(
    mongo_Connector _this,
    corto_request *request);
#define mongo_Connector_onRequest(_this, request) _mongo_Connector_onRequest(mongo_Connector(_this), request)

#ifdef __cplusplus
}
#endif
#endif

