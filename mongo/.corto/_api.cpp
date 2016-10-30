/* _api.cpp
 *
 * API convenience functions for C-language.
 * This file contains generated code. Do not modify!
 */

#include <remuv/mongo/mongo.h>
mongo_Connector _mongo_ConnectorCreate(corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port) {
    mongo_Connector _this;
    _this = mongo_Connector(corto_declare(mongo_Connector_o));
    if (!_this) {
        return NULL;
    }
    if (!corto_checkState(_this, CORTO_DEFINED)) {
        corto_setref(&((corto_mount)_this)->mount, mount);
        ((corto_mount)_this)->mask = mask;
        corto_setstr(&((mongo_Connector)_this)->dbname, dbname);
        corto_setstr(&((mongo_Connector)_this)->user, user);
        corto_setstr(&((mongo_Connector)_this)->password, password);
        corto_setstr(&((mongo_Connector)_this)->hostaddr, hostaddr);
        ((mongo_Connector)_this)->port = port;
        if (corto_define(_this)) {
            corto_release(_this);
            _this = NULL;
        }
    }
    return _this;
}

mongo_Connector _mongo_ConnectorCreateChild(corto_object _parent, corto_string _name, corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port) {
    mongo_Connector _this;
    _this = mongo_Connector(corto_declareChild(_parent, _name, mongo_Connector_o));
    if (!_this) {
        return NULL;
    }
    if (!corto_checkState(_this, CORTO_DEFINED)) {
        corto_setref(&((corto_mount)_this)->mount, mount);
        ((corto_mount)_this)->mask = mask;
        corto_setstr(&((mongo_Connector)_this)->dbname, dbname);
        corto_setstr(&((mongo_Connector)_this)->user, user);
        corto_setstr(&((mongo_Connector)_this)->password, password);
        corto_setstr(&((mongo_Connector)_this)->hostaddr, hostaddr);
        ((mongo_Connector)_this)->port = port;
        if (corto_define(_this)) {
            corto_release(_this);
            _this = NULL;
        }
    }
    return _this;
}

corto_int16 _mongo_ConnectorUpdate(mongo_Connector _this, corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port) {
    CORTO_UNUSED(_this);
    if (!corto_updateBegin(_this)) {
        corto_setref(&((corto_mount)_this)->mount, mount);
        ((corto_mount)_this)->mask = mask;
        corto_setstr(&((mongo_Connector)_this)->dbname, dbname);
        corto_setstr(&((mongo_Connector)_this)->user, user);
        corto_setstr(&((mongo_Connector)_this)->password, password);
        corto_setstr(&((mongo_Connector)_this)->hostaddr, hostaddr);
        ((mongo_Connector)_this)->port = port;
        corto_updateEnd(_this);
    } else {
        return -1;
    }
    return 0;
}

mongo_Connector _mongo_ConnectorDeclare(void) {
    mongo_Connector _this;
    _this = mongo_Connector(corto_declare(mongo_Connector_o));
    if (!_this) {
        return NULL;
    }
    return _this;
}

mongo_Connector _mongo_ConnectorDeclareChild(corto_object _parent, corto_string _name) {
    mongo_Connector _this;
    _this = mongo_Connector(corto_declareChild(_parent, _name, mongo_Connector_o));
    if (!_this) {
        return NULL;
    }
    return _this;
}

corto_int16 _mongo_ConnectorDefine(mongo_Connector _this, corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port) {
    CORTO_UNUSED(_this);
    corto_setref(&((corto_mount)_this)->mount, mount);
    ((corto_mount)_this)->mask = mask;
    corto_setstr(&((mongo_Connector)_this)->dbname, dbname);
    corto_setstr(&((mongo_Connector)_this)->user, user);
    corto_setstr(&((mongo_Connector)_this)->password, password);
    corto_setstr(&((mongo_Connector)_this)->hostaddr, hostaddr);
    ((mongo_Connector)_this)->port = port;
    return corto_define(_this);
}

mongo_Connector _mongo_ConnectorAssign(mongo_Connector _this, corto_object mount, corto_eventMask mask, corto_string dbname, corto_string user, corto_string password, corto_string hostaddr, corto_int32 port) {
    CORTO_UNUSED(_this);
    corto_setref(&((corto_mount)_this)->mount, mount);
    ((corto_mount)_this)->mask = mask;
    corto_setstr(&((mongo_Connector)_this)->dbname, dbname);
    corto_setstr(&((mongo_Connector)_this)->user, user);
    corto_setstr(&((mongo_Connector)_this)->password, password);
    corto_setstr(&((mongo_Connector)_this)->hostaddr, hostaddr);
    ((mongo_Connector)_this)->port = port;
    return _this;
}

corto_string _mongo_ConnectorStr(mongo_Connector value) {
    corto_string result;
    corto_value v;
    v = corto_value_object(value, corto_type(mongo_Connector_o));
    result = corto_strv(&v, 0);
    return result;
}

mongo_Connector mongo_ConnectorFromStr(mongo_Connector value, corto_string str) {
    corto_fromStrp(&value, corto_type(mongo_Connector_o), str);
    return value;
}

corto_equalityKind _mongo_ConnectorCompare(mongo_Connector dst, mongo_Connector src) {
    return corto_compare(dst, src);
}

