/* Connector.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef CORTO_DDS_CONNECTOR_H
#define CORTO_DDS_CONNECTOR_H

#include <corto/corto.h>
#include <corto/dds/_interface.h>
#include <corto/dds/_type.h>
#include <corto/dds/_api.h>
#include <corto/dds/_meta.h>

#ifdef __cplusplus
extern "C" {
#endif


CORTO_DDS_EXPORT corto_int16 _dds_Connector_construct(
    dds_Connector _this);
#define dds_Connector_construct(_this) _dds_Connector_construct(dds_Connector(_this))

CORTO_DDS_EXPORT corto_void _dds_Connector_destruct(
    dds_Connector _this);
#define dds_Connector_destruct(_this) _dds_Connector_destruct(dds_Connector(_this))

CORTO_DDS_EXPORT corto_void _dds_Connector_onDeclare(
    dds_Connector _this,
    corto_object observable);
#define dds_Connector_onDeclare(_this, observable) _dds_Connector_onDeclare(dds_Connector(_this), observable)

CORTO_DDS_EXPORT corto_void _dds_Connector_onDelete(
    dds_Connector _this,
    corto_object observable);
#define dds_Connector_onDelete(_this, observable) _dds_Connector_onDelete(dds_Connector(_this), observable)

CORTO_DDS_EXPORT corto_object _dds_Connector_onResume(
    dds_Connector _this,
    corto_string parent,
    corto_string name,
    corto_object o);
#define dds_Connector_onResume(_this, parent, name, o) _dds_Connector_onResume(dds_Connector(_this), parent, name, o)

CORTO_DDS_EXPORT corto_void _dds_Connector_onUpdate(
    dds_Connector _this,
    corto_object observable);
#define dds_Connector_onUpdate(_this, observable) _dds_Connector_onUpdate(dds_Connector(_this), observable)

CORTO_DDS_EXPORT corto_void _dds_Connector_sendRequest(
    dds_Connector _this,
    corto_string name,
    corto_string type,
    corto_string value);
#define dds_Connector_sendRequest(_this, name, type, value) _dds_Connector_sendRequest(dds_Connector(_this), name, type, value)

#ifdef __cplusplus
}
#endif
#endif

