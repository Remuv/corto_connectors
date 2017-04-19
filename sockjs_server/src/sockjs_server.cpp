/* $CORTO_GENERATED
 *
 * sockjs_server.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/sockjsserver/sockjsserver.h>

corto_int16 _sockjsserver_sockjs_server_construct(
    sockjsserver_sockjs_server _this)
{
/* $begin(recorto/sockjsserver/sockjs_server/construct) */
    return server_SockJs_construct(_this);
/* $end */
}

corto_void _sockjsserver_sockjs_server_onAdded(
    sockjsserver_sockjs_server _this,
    corto_eventMask event,
    corto_object object,
    corto_observer observer)
{
/* $begin(recorto/sockjsserver/sockjs_server/onAdded) */

/* $end */
}

corto_void _sockjsserver_sockjs_server_onChanged(
    sockjsserver_sockjs_server _this,
    corto_eventMask event,
    corto_object object,
    corto_observer observer)
{
/* $begin(recorto/sockjsserver/sockjs_server/onChanged) */

/* $end */
}

corto_void _sockjsserver_sockjs_server_onClose(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c)
{
/* $begin(recorto/sockjsserver/sockjs_server/onClose) */
    if(_this->handler == 0 || _this->handler_func == 0)
    {
        return;
    }
    void (*fptr)(corto_object,sockjsserver_sockjs_server,server_HTTP_Connection, corto_string);
    *(void **)(&fptr) = (void*)_this->handler_func;
    fptr((void*)_this->handler, _this, c, "{\"type\":\"CONNECTION_CLOSE\"}");
/* $end */
}

corto_void _sockjsserver_sockjs_server_onData(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(recorto/sockjsserver/sockjs_server/onData) */

/* $end */
}

corto_void _sockjsserver_sockjs_server_onMessage(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(recorto/sockjsserver/sockjs_server/onMessage) */
    if(_this->handler == 0 || _this->handler_func == 0)
    {
        return;
    }
    void (*fptr)(corto_object,sockjsserver_sockjs_server,server_HTTP_Connection, corto_string);
    *(void **)(&fptr) = (void*)_this->handler_func;
    fptr((void*)_this->handler, _this, c, msg);
/* $end */
}

corto_void _sockjsserver_sockjs_server_onOpen(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c)
{
/* $begin(recorto/sockjsserver/sockjs_server/onOpen) */
    if(_this->handler == 0 || _this->handler_func == 0)
    {
        return;
    }
    void (*fptr)(corto_object,sockjsserver_sockjs_server,server_HTTP_Connection, corto_string);
    *(void **)(&fptr) = (void*)_this->handler_func;
    fptr((void*)_this->handler, _this, c, "{\"type\":\"CONNECTION_OPEN\"}");
/* $end */
}

corto_void _sockjsserver_sockjs_server_onPoll(
    sockjsserver_sockjs_server _this)
{
/* $begin(recorto/sockjsserver/sockjs_server/onPoll) */

/* $end */
}

corto_void _sockjsserver_sockjs_server_onRemoved(
    sockjsserver_sockjs_server _this,
    corto_eventMask event,
    corto_object object,
    corto_observer observer)
{
/* $begin(recorto/sockjsserver/sockjs_server/onRemoved) */

/* $end */
}

corto_void _sockjsserver_sockjs_server_sendMessage(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(recorto/sockjsserver/sockjs_server/sendMessage) */
    server_HTTP_write(server_Service(_this)->server, c, msg);
/* $end */
}

corto_void _sockjsserver_sockjs_server_setHandler(
    sockjsserver_sockjs_server _this,
    corto_object obj,
    corto_word func)
{
/* $begin(recorto/sockjsserver/sockjs_server/setHandler) */
    _this->handler = (corto_word)obj;
    _this->handler_func = func;
/* $end */
}
