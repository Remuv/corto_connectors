/* Connector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef RECORTO_CLMDB_CONNECTOR_H
#define RECORTO_CLMDB_CONNECTOR_H

#include <corto/corto.h>
#include <recorto/clmdb/_interface.h>
#include <recorto/clmdb/_type.h>
#include <recorto/clmdb/_api.h>
#include <recorto/clmdb/_meta.h>

#ifdef __cplusplus
extern "C" {
#endif


RECORTO_CLMDB_EXPORT corto_int16 _clmdb_Connector_construct(
    clmdb_Connector _this);
#define clmdb_Connector_construct(_this) _clmdb_Connector_construct(clmdb_Connector(_this))

RECORTO_CLMDB_EXPORT corto_void _clmdb_Connector_destruct(
    clmdb_Connector _this);
#define clmdb_Connector_destruct(_this) _clmdb_Connector_destruct(clmdb_Connector(_this))

RECORTO_CLMDB_EXPORT corto_void _clmdb_Connector_onDelete(
    clmdb_Connector _this,
    corto_object observable);
#define clmdb_Connector_onDelete(_this, observable) _clmdb_Connector_onDelete(clmdb_Connector(_this), observable)

RECORTO_CLMDB_EXPORT corto_object _clmdb_Connector_onResume(
    clmdb_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object o);
#define clmdb_Connector_onResume(_this, parent, name, o) _clmdb_Connector_onResume(clmdb_Connector(_this), parent, name, o)

RECORTO_CLMDB_EXPORT corto_void _clmdb_Connector_onUpdate(
    clmdb_Connector _this,
    corto_object observable);
#define clmdb_Connector_onUpdate(_this, observable) _clmdb_Connector_onUpdate(clmdb_Connector(_this), observable)

#ifdef __cplusplus
}
#endif
#endif

