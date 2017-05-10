/* $CORTO_GENERATED
 *
 * common.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/common/common.h>

/* $header() */
#include <mutex>
void FreeCB(void*b){}

corto_uint8 OLS_COPY_CALLBACK_KEY()
{
    static corto_uint8 RECORTO_OLS_COPY_CALLBACK = 0;
    static std::once_flag ols_initFlag;

    std::call_once(ols_initFlag, [&](){
        RECORTO_OLS_COPY_CALLBACK = corto_olsKey(FreeCB);
    });

    return RECORTO_OLS_COPY_CALLBACK;
}

/* $end */

int commonMain(int argc, char *argv[]) {
/* $begin(main) */

    return 0;
/* $end */
}
