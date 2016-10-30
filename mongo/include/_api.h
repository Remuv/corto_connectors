/* _api.h
 *
 * API convenience functions for C-language.
 * This file contains generated code. Do not modify!
 */

#ifndef REMUV_MONGO__API_H
#define REMUV_MONGO__API_H

#include <corto/corto.h>
#include <remuv/mongo/_interface.h>
#ifdef __cplusplus
extern "C" {
#endif
/* /remuv/mongo/Connector */
REMUV_MONGO_EXPORT mongo_Connector _mongo_ConnectorCreate(corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port);
#define mongo_ConnectorCreate(mount, mask, dbname, user, password, hostaddr, port) _mongo_ConnectorCreate(mount, mask, dbname, user, password, hostaddr, port)
#define mongo_ConnectorCreate_auto(_name, mount, mask, dbname, user, password, hostaddr, port) mongo_Connector _name = mongo_ConnectorCreate(mount, mask, dbname, user, password, hostaddr, port); (void)_name
REMUV_MONGO_EXPORT mongo_Connector _mongo_ConnectorCreateChild(corto_object _parent, corto_string _name, corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port);
#define mongo_ConnectorCreateChild(_parent, _name, mount, mask, dbname, user, password, hostaddr, port) _mongo_ConnectorCreateChild(_parent, _name, mount, mask, dbname, user, password, hostaddr, port)
#define mongo_ConnectorCreateChild_auto(_parent, _name, mount, mask, dbname, user, password, hostaddr, port) mongo_Connector _name = mongo_ConnectorCreateChild(_parent, #_name, mount, mask, dbname, user, password, hostaddr, port); (void)_name
REMUV_MONGO_EXPORT corto_int16 _mongo_ConnectorUpdate(mongo_Connector _this, corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port);
#define mongo_ConnectorUpdate(_this, mount, mask, dbname, user, password, hostaddr, port) _mongo_ConnectorUpdate(mongo_Connector(_this), mount, mask, dbname, user, password, hostaddr, port)

REMUV_MONGO_EXPORT mongo_Connector _mongo_ConnectorDeclare(void);
#define mongo_ConnectorDeclare() _mongo_ConnectorDeclare()
#define mongo_ConnectorDeclare_auto(_name) mongo_Connector _name = mongo_ConnectorDeclare(); (void)_name
REMUV_MONGO_EXPORT mongo_Connector _mongo_ConnectorDeclareChild(corto_object _parent, corto_string _name);
#define mongo_ConnectorDeclareChild(_parent, _name) _mongo_ConnectorDeclareChild(_parent, _name)
#define mongo_ConnectorDeclareChild_auto(_parent, _name) mongo_Connector _name = mongo_ConnectorDeclareChild(_parent, #_name); (void)_name
REMUV_MONGO_EXPORT corto_int16 _mongo_ConnectorDefine(mongo_Connector _this, corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port);
#define mongo_ConnectorDefine(_this, mount, mask, dbname, user, password, hostaddr, port) _mongo_ConnectorDefine(mongo_Connector(_this), mount, mask, dbname, user, password, hostaddr, port)
REMUV_MONGO_EXPORT mongo_Connector _mongo_ConnectorAssign(mongo_Connector _this, corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port);
#define mongo_Connector__optional_NotSet NULL
#define mongo_Connector__optional_Set(mount, mask, dbname, user, password, hostaddr, port) mongo_ConnectorAssign((mongo_Connector*)corto_calloc(sizeof(mongo_Connector)), mount, mask, dbname, user, password, hostaddr, port)
#define mongo_Connector__optional_SetCond(cond, mount, mask, dbname, user, password, hostaddr, port) cond ? mongo_ConnectorAssign((mongo_Connector*)corto_calloc(sizeof(mongo_Connector)), mount, mask, dbname, user, password, hostaddr, port) : NULL
#define mongo_ConnectorUnset(_this) _this ? corto_deinitp(_this, mongo_Connector_o) : 0; corto_dealloc(_this); _this = NULL;
#define mongo_ConnectorAssign(_this, mount, mask, dbname, user, password, hostaddr, port) _mongo_ConnectorAssign(mongo_Connector(_this), mount, mask, dbname, user, password, hostaddr, port)
#define mongo_ConnectorSet(_this, mount, mask, dbname, user, password, hostaddr, port) _this = _this ? _this : (mongo_Connector*)corto_calloc(sizeof(mongo_Connector)); _mongo_ConnectorAssign(mongo_Connector(_this), mount, mask, dbname, user, password, hostaddr, port)
REMUV_MONGO_EXPORT corto_string _mongo_ConnectorStr(mongo_Connector value);
#define mongo_ConnectorStr(value) _mongo_ConnectorStr(mongo_Connector(value))
REMUV_MONGO_EXPORT mongo_Connector mongo_ConnectorFromStr(mongo_Connector value, corto_string str);
REMUV_MONGO_EXPORT corto_equalityKind _mongo_ConnectorCompare(mongo_Connector dst, mongo_Connector src);
#define mongo_ConnectorCompare(dst, src) _mongo_ConnectorCompare(mongo_Connector(dst), mongo_Connector(src))


#ifdef __cplusplus
}
#endif
#endif

