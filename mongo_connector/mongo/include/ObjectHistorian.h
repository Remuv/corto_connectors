/* ObjectHistorian.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef RECORTO_MONGO_CONNECTOR_MONGO_OBJECTHISTORIAN_H
#define RECORTO_MONGO_CONNECTOR_MONGO_OBJECTHISTORIAN_H

#include <corto/corto.h>
#include <corto/corto.h>
#include <recorto/mongo_connector/mongo/_project.h>

#include <recorto/mongo_connector/mongo/_type.h>
#include <recorto/mongo_connector/mongo/_load.h>
#include <recorto/mongo_connector/mongo/_api.h>

#ifdef __cplusplus
extern "C" {
#endif

#define mongo_ObjectHistorian_construct(_this) _mongo_ObjectHistorian_construct(mongo_ObjectHistorian(_this))
RECORTO_MONGO_CONNECTOR_MONGO_EXPORT
corto_int16 _mongo_ObjectHistorian_construct(
    mongo_ObjectHistorian _this);

#define mongo_ObjectHistorian_destruct(_this) _mongo_ObjectHistorian_destruct(mongo_ObjectHistorian(_this))
RECORTO_MONGO_CONNECTOR_MONGO_EXPORT
corto_void _mongo_ObjectHistorian_destruct(
    mongo_ObjectHistorian _this);

#define mongo_ObjectHistorian_onNotify(_this, event, object) _mongo_ObjectHistorian_onNotify(mongo_ObjectHistorian(_this), event, object)
RECORTO_MONGO_CONNECTOR_MONGO_EXPORT
corto_void _mongo_ObjectHistorian_onNotify(
    mongo_ObjectHistorian _this,
    corto_eventMask event,
    corto_result *object);

#define mongo_ObjectHistorian_onRequest(_this, request) _mongo_ObjectHistorian_onRequest(mongo_ObjectHistorian(_this), request)
RECORTO_MONGO_CONNECTOR_MONGO_EXPORT
corto_resultIter _mongo_ObjectHistorian_onRequest(
    mongo_ObjectHistorian _this,
    corto_request *request);

#ifdef __cplusplus
}
#endif

#endif

