/* _type.h
 *
 * Type definitions for C-language.
 * This file contains generated code. Do not modify!
 */

#ifndef REMUV_MONGO__TYPE_H
#define REMUV_MONGO__TYPE_H

#include <corto/corto.h>
#include <remuv/_type.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Casting macro's */
#define mongo_Connector(o) ((mongo_Connector)corto_assertType((corto_type)mongo_Connector_o, o))

/* Type definitions */
/*  /remuv/mongo/Connector */
CORTO_CLASS(mongo_Connector);

CORTO_CLASS_DEF(mongo_Connector) {
    CORTO_EXTEND(corto_mount);
    corto_string dbname;
    corto_string user;
    corto_string password;
    corto_string hostaddr;
    corto_int32 port;
};

#ifdef __cplusplus
}
#endif
#endif

