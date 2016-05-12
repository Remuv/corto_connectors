/* $CORTO_GENERATED
 *
 * MongoDb.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <test.h>

/* $header() */
#include <corto/mongodb/mongo_util.h>

/* $end */

corto_void _test_MongoDb_StartTest(
    test_MongoDb _this)
{
/* $begin(test/MongoDb/StartTest) */
    corto_object mydb = corto_voidCreateChild(root_o, "root" );
    mongodb_Connector mongo = mongodb_ConnectorCreateChild(root_o, "mydb", mydb, CORTO_ON_SCOPE | CORTO_ON_TREE, "mydb", "data","rin","/test/Data",  "admin", "abc123", "127.0.0.1", 27017);

    test_TestData obj2 = (test_TestData)corto_resolve(mydb, "MyUri.Home.Test2");

    test_Data d1 = test_DataCreateChild(mydb, "MyUri.Home.Test", "MyUri.Home.Test");
    test_Data d2 = test_DataCreateChild(mydb, "MyUri.Home.Test2", "MyUri.Home.Test2");
    test_TestData d3 = test_TestDataCreateChild(mydb, "MyUri.Home.Test3", "MyUri.Home.Test3", "a1", "a2", "a3");

    corto_int16 ret = 0;
    corto_iter iter = corto_select("root", "*").contentType("text/json").iter( &ret );

    while(corto_iterHasNext(&iter))
    {
        corto_result *result = (corto_result*)corto_iterNext(&iter);
    }
    
    corto_trace("exit");
    test_assert(true);
/* $end */
}
