/* sockjs_server.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef RECORTO_SOCKJSSERVER_SOCKJS_SERVER_H
#define RECORTO_SOCKJSSERVER_SOCKJS_SERVER_H

#include <corto/corto.h>
#include <recorto/sockjsserver/_project.h>
#include <recorto/sockjsserver/_type.h>

#ifdef __cplusplus
extern "C" {
#endif


RECORTO_SOCKJSSERVER_EXPORT corto_int16 _sockjsserver_sockjs_server_construct(
    sockjsserver_sockjs_server _this);
#define sockjsserver_sockjs_server_construct(_this) _sockjsserver_sockjs_server_construct(sockjsserver_sockjs_server(_this))

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_onAdded(
    sockjsserver_sockjs_server _this,
    corto_eventMask event,
    corto_object object,
    corto_observer observer);
#define sockjsserver_sockjs_server_onAdded(_this, event, object, observer) _sockjsserver_sockjs_server_onAdded(sockjsserver_sockjs_server(_this), event, object, corto_observer(observer))

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_onChanged(
    sockjsserver_sockjs_server _this,
    corto_eventMask event,
    corto_object object,
    corto_observer observer);
#define sockjsserver_sockjs_server_onChanged(_this, event, object, observer) _sockjsserver_sockjs_server_onChanged(sockjsserver_sockjs_server(_this), event, object, corto_observer(observer))

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_onClose(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c);
#define sockjsserver_sockjs_server_onClose(_this, c) _sockjsserver_sockjs_server_onClose(sockjsserver_sockjs_server(_this), server_HTTP_Connection(c))

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_onData(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c,
    corto_string msg);
#define sockjsserver_sockjs_server_onData(_this, c, msg) _sockjsserver_sockjs_server_onData(sockjsserver_sockjs_server(_this), server_HTTP_Connection(c), msg)

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_onMessage(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c,
    corto_string msg);
#define sockjsserver_sockjs_server_onMessage(_this, c, msg) _sockjsserver_sockjs_server_onMessage(sockjsserver_sockjs_server(_this), server_HTTP_Connection(c), msg)

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_onOpen(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c);
#define sockjsserver_sockjs_server_onOpen(_this, c) _sockjsserver_sockjs_server_onOpen(sockjsserver_sockjs_server(_this), server_HTTP_Connection(c))

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_onPoll(
    sockjsserver_sockjs_server _this);
#define sockjsserver_sockjs_server_onPoll(_this) _sockjsserver_sockjs_server_onPoll(sockjsserver_sockjs_server(_this))

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_onRemoved(
    sockjsserver_sockjs_server _this,
    corto_eventMask event,
    corto_object object,
    corto_observer observer);
#define sockjsserver_sockjs_server_onRemoved(_this, event, object, observer) _sockjsserver_sockjs_server_onRemoved(sockjsserver_sockjs_server(_this), event, object, corto_observer(observer))

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_sendMessage(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c,
    corto_string msg);
#define sockjsserver_sockjs_server_sendMessage(_this, c, msg) _sockjsserver_sockjs_server_sendMessage(sockjsserver_sockjs_server(_this), server_HTTP_Connection(c), msg)

RECORTO_SOCKJSSERVER_EXPORT corto_void _sockjsserver_sockjs_server_setHandler(
    sockjsserver_sockjs_server _this,
    corto_object obj,
    corto_word func);
#define sockjsserver_sockjs_server_setHandler(_this, obj, func) _sockjsserver_sockjs_server_setHandler(sockjsserver_sockjs_server(_this), obj, func)

#ifdef __cplusplus
}
#endif
#endif

