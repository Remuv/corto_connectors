#include <recorto/data_models/data_models.h>

void data_models_server_DataCopy(corto_object *dst, corto_object src)
{
    if (src == nullptr)
    {
        return;
    }

    data_models_server_Data obj = data_models_server_Data(src);
    data_models_server_Data *dest = (data_models_server_Data*)dst;

    if (*dest == nullptr)
    {
        *dest = data_models_server_DataCreate(obj->interface, obj->type, obj->rin, obj->alias, obj->access, obj->__v);
        if (*dest == nullptr){corto_setstr(&(*dest)->_id, obj->_id);}
    }
    else
    {
        data_models_server_DataAssign(*dest, obj->interface, obj->type, obj->rin, obj->alias, obj->access, obj->__v);
    }
}

void data_models_sockjs_DataCopy(corto_object *dst, corto_object src)
{
    if (src == nullptr)
    {
        return;
    }

    data_models_sockjs_Data obj = data_models_sockjs_Data(src);
    data_models_sockjs_Data *dest = (data_models_sockjs_Data*)dst;

    if (*dest == nullptr)
    {
        *dest = data_models_sockjs_DataCreate(obj->type, obj->uuid, obj->value);
        
    }
    else
    {
        data_models_sockjs_DataAssign(*dest, obj->type, obj->uuid, obj->value);
    }
}

void data_models_net_ValueTypeCopy(corto_object *dst, corto_object src)
{
    if (src == nullptr)
    {
        return;
    }

    data_models_net_ValueType* obj = data_models_net_ValueType(src);
    data_models_net_ValueType* *dest = (data_models_net_ValueType**)dst;

    if (*dest == nullptr)
    {
        *dest = data_models_net_ValueTypeCreate(obj->hint, obj->str, obj->val);
        
    }
    else
    {
        data_models_net_ValueTypeAssign(*dest, obj->hint, obj->str, obj->val);
    }
}

void data_models_net_DataCopy(corto_object *dst, corto_object src)
{
    if (src == nullptr)
    {
        return;
    }

    data_models_net_Data obj = data_models_net_Data(src);
    data_models_net_Data *dest = (data_models_net_Data*)dst;

    if (*dest == nullptr)
    {
        *dest = data_models_net_DataCreate(obj->hint, &obj->actual, &obj->target, obj->_callback);
        
    }
    else
    {
        data_models_net_DataAssign(*dest, obj->hint, &obj->actual, &obj->target, obj->_callback);
    }
}

void data_models_object_ptrCopy(corto_object *dst, corto_object src)
{
    if (src == nullptr)
    {
        return;
    }

    data_models_object_ptr* obj = data_models_object_ptr(src);
    data_models_object_ptr* *dest = (data_models_object_ptr**)dst;

    if (*dest == nullptr)
    {
        *dest = data_models_object_ptrCreate(obj->ptr);
        
    }
    else
    {
        data_models_object_ptrAssign(*dest, obj->ptr);
    }
}

void LoadReCortoDataModelsCopyCallbacks()
{
	CORTO_OLS_SET_COPY_CB(data_models_server_Data);
	CORTO_OLS_SET_COPY_CB(data_models_sockjs_Data);
	CORTO_OLS_SET_COPY_CB(data_models_net_ValueType);
	CORTO_OLS_SET_COPY_CB(data_models_net_Data);
	CORTO_OLS_SET_COPY_CB(data_models_object_ptr);
}
