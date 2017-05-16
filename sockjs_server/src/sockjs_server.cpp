/* $CORTO_GENERATED
 *
 * sockjs_server.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/sockjsserver/sockjsserver.h>

void _sockjsserver_sockjs_server_onClose(
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

void _sockjsserver_sockjs_server_onMessage(
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

void _sockjsserver_sockjs_server_onOpen(
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

void _sockjsserver_sockjs_server_sendMessage(
    sockjsserver_sockjs_server _this,
    server_HTTP_Connection c,
    corto_string msg)
{
/* $begin(recorto/sockjsserver/sockjs_server/sendMessage) */
    server_HTTP_write(server_Service(_this)->server, c, msg);
/* $end */
}

void _sockjsserver_sockjs_server_setHandler(
    sockjsserver_sockjs_server _this,
    corto_object obj,
    uintptr_t func)
{
/* $begin(recorto/sockjsserver/sockjs_server/setHandler) */
    _this->handler = (corto_word)obj;
    _this->handler_func = func;
/* $end */
}
