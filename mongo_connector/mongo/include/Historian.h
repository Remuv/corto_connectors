/* Historian.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef RECORTO_MONGO_CONNECTOR_MONGO_HISTORIAN_H
#define RECORTO_MONGO_CONNECTOR_MONGO_HISTORIAN_H

#include <corto/corto.h>
#include <recorto/mongo_connector/mongo/_project.h>
#include <recorto/mongo_connector/mongo/_type.h>

#ifdef __cplusplus
extern "C" {
#endif


RECORTO_MONGO_CONNECTOR_MONGO_EXPORT corto_int16 _mongo_Historian_construct(
    mongo_Historian _this);
#define mongo_Historian_construct(_this) _mongo_Historian_construct(mongo_Historian(_this))

RECORTO_MONGO_CONNECTOR_MONGO_EXPORT corto_void _mongo_Historian_destruct(
    mongo_Historian _this);
#define mongo_Historian_destruct(_this) _mongo_Historian_destruct(mongo_Historian(_this))

RECORTO_MONGO_CONNECTOR_MONGO_EXPORT corto_void _mongo_Historian_onNotify(
    mongo_Historian _this,
    corto_eventMask event,
    corto_result *object);
#define mongo_Historian_onNotify(_this, event, object) _mongo_Historian_onNotify(mongo_Historian(_this), event, object)

RECORTO_MONGO_CONNECTOR_MONGO_EXPORT corto_resultIter _mongo_Historian_onRequest(
    mongo_Historian _this,
    corto_request *request);
#define mongo_Historian_onRequest(_this, request) _mongo_Historian_onRequest(mongo_Historian(_this), request)

#ifdef __cplusplus
}
#endif
#endif

