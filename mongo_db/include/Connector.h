/* Connector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef CORTO_MONGODB_CONNECTOR_H
#define CORTO_MONGODB_CONNECTOR_H

#include <corto/corto.h>
#include <corto/mongodb/_interface.h>
#include <corto/mongodb/_type.h>
#include <corto/mongodb/_api.h>
#include <corto/mongodb/_meta.h>

#ifdef __cplusplus
extern "C" {
#endif


CORTO_MONGODB_EXPORT corto_int16 _mongodb_Connector_construct(
    mongodb_Connector _this);
#define mongodb_Connector_construct(_this) _mongodb_Connector_construct(mongodb_Connector(_this))

CORTO_MONGODB_EXPORT corto_string _mongodb_Connector_FindById(
    mongodb_Connector _this,
    corto_string id);
#define mongodb_Connector_FindById(_this, id) _mongodb_Connector_FindById(mongodb_Connector(_this), id)

CORTO_MONGODB_EXPORT mongodb_stringList _mongodb_Connector_FindByIds(
    mongodb_Connector _this,
    mongodb_stringList ids);
#define mongodb_Connector_FindByIds(_this, ids) _mongodb_Connector_FindByIds(mongodb_Connector(_this), ids)

CORTO_MONGODB_EXPORT corto_void _mongodb_Connector_onDeclare(
    mongodb_Connector _this,
    corto_object observable);
#define mongodb_Connector_onDeclare(_this, observable) _mongodb_Connector_onDeclare(mongodb_Connector(_this), observable)

CORTO_MONGODB_EXPORT corto_void _mongodb_Connector_onDelete(
    mongodb_Connector _this,
    corto_object observable);
#define mongodb_Connector_onDelete(_this, observable) _mongodb_Connector_onDelete(mongodb_Connector(_this), observable)

CORTO_MONGODB_EXPORT corto_resultIter _mongodb_Connector_onRequest(
    mongodb_Connector _this,
    corto_request *request);
#define mongodb_Connector_onRequest(_this, request) _mongodb_Connector_onRequest(mongodb_Connector(_this), request)

CORTO_MONGODB_EXPORT corto_object _mongodb_Connector_onResume(
    mongodb_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object o);
#define mongodb_Connector_onResume(_this, parent, name, o) _mongodb_Connector_onResume(mongodb_Connector(_this), parent, name, o)

CORTO_MONGODB_EXPORT corto_void _mongodb_Connector_onUpdate(
    mongodb_Connector _this,
    corto_object observable);
#define mongodb_Connector_onUpdate(_this, observable) _mongodb_Connector_onUpdate(mongodb_Connector(_this), observable)

#ifdef __cplusplus
}
#endif
#endif

