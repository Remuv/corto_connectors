/* Connector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef RECORTO_DDS_CONNECTOR_H
#define RECORTO_DDS_CONNECTOR_H

#include <corto/corto.h>
#include <recorto/dds/_project.h>
#include <recorto/dds/_type.h>
#include <recorto/dds/_api.h>
#include <recorto/dds/_load.h>

#ifdef __cplusplus
extern "C" {
#endif


RECORTO_DDS_EXPORT corto_int16 _dds_Connector_construct(
    dds_Connector _this);
#define dds_Connector_construct(_this) _dds_Connector_construct(dds_Connector(_this))

RECORTO_DDS_EXPORT corto_void _dds_Connector_destruct(
    dds_Connector _this);
#define dds_Connector_destruct(_this) _dds_Connector_destruct(dds_Connector(_this))

RECORTO_DDS_EXPORT corto_void _dds_Connector_onDeclare(
    dds_Connector _this,
    corto_object observable);
#define dds_Connector_onDeclare(_this, observable) _dds_Connector_onDeclare(dds_Connector(_this), observable)

RECORTO_DDS_EXPORT corto_void _dds_Connector_onDelete(
    dds_Connector _this,
    corto_object observable);
#define dds_Connector_onDelete(_this, observable) _dds_Connector_onDelete(dds_Connector(_this), observable)

RECORTO_DDS_EXPORT corto_object _dds_Connector_onResume(
    dds_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object o);
#define dds_Connector_onResume(_this, parent, name, o) _dds_Connector_onResume(dds_Connector(_this), parent, name, o)

RECORTO_DDS_EXPORT corto_void _dds_Connector_onUpdate(
    dds_Connector _this,
    corto_object observable);
#define dds_Connector_onUpdate(_this, observable) _dds_Connector_onUpdate(dds_Connector(_this), observable)

RECORTO_DDS_EXPORT corto_void _dds_Connector_sendRequest(
    dds_Connector _this,
    corto_string name,
    corto_string type,
    corto_string value);
#define dds_Connector_sendRequest(_this, name, type, value) _dds_Connector_sendRequest(dds_Connector(_this), name, type, value)

#ifdef __cplusplus
}
#endif
#endif

