/* $CORTO_GENERATED
 *
 * ddsConnectorTest.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <test.h>

/* $header() */
void OnNotify (
    corto_object _this,
    corto_eventMask event,
    corto_result *result,
    corto_subscriber subscriber
)
{
    printf("OnNotify p = %s, id = %s, t = %s, v = %s\n", result->parent, result->id, result->type, (char*)result->value );
}

void OnDelete (
    corto_object _this,
    corto_eventMask event,
    corto_result *result,
    corto_subscriber subscriber
)
{
    printf("OnDelete p = %s, id = %s, t = %s, v = %s\n", result->parent, result->id, result->type, (char*)result->value );
}

/* $end */

corto_void _test_ddsConnectorTest_StartTest(
    test_ddsConnectorTest _this)
{
/* $begin(test/ddsConnectorTest/StartTest) */
    corto_time timeout = {60, 0};
    test_setTimeout(&timeout);

    corto_verbosity(corto_err::CORTO_TRACE);
    corto_object myRoot = corto_voidCreateChild(root_o, "myRoot");

    corto_object mount = corto_voidCreateChild(myRoot, "mount");
    corto_object sync_mount = corto_voidCreateChild(myRoot, "sync");

    corto_trace("Create DDS Connector on 'mount'");
    dds_Connector net = dds_ConnectorCreateChild(root_o, "net", mount, "SyncTest");
    dds_Connector sync = dds_ConnectorCreateChild(root_o, "sync_net", sync_mount, "SyncTest");


    corto_object A = corto_int64CreateChild(mount, "A", 1); usleep(1000*100);

     corto_object Aa = corto_int64CreateChild(A, "a", 11); usleep(1000*100);
     corto_object Ab = corto_int64CreateChild(A, "b", 12); usleep(1000*100);
     corto_object Ac = corto_int64CreateChild(A, "c", 13); usleep(1000*100);
     corto_object Ad = corto_int64CreateChild(A, "d", 14); usleep(1000*100);
     corto_object Ae = corto_int64CreateChild(A, "e", 15); usleep(1000*100);
      corto_object Aea = corto_int64CreateChild(Ae, "a", 16); usleep(1000*100);
      corto_object Aeb = corto_int64CreateChild(Ae, "b", 17); usleep(1000*100);
      corto_object Aec = corto_int64CreateChild(Ae, "c", 18); usleep(1000*100);
      corto_object Aed = corto_int64CreateChild(Ae, "d", 19); usleep(1000*100);


    corto_object B = corto_int64CreateChild(mount, "B", 2); usleep(1000*100);
     corto_object Ba = corto_int64CreateChild(B, "a", 21); usleep(1000*100);
     corto_object Bb = corto_int64CreateChild(B, "b", 22); usleep(1000*100);
     corto_object Bc = corto_int64CreateChild(B, "c", 23); usleep(1000*100);
     corto_object Bd = corto_int64CreateChild(B, "d", 24); usleep(1000*100);
     corto_object Be = corto_int64CreateChild(B, "e", 25); usleep(1000*100);



    corto_subscriber subscriber = corto_subscribe(CORTO_ON_UPDATE, "/myRoot", "mount/A")
                                        .contentType("text/json")
                                        .callback(OnNotify);
    corto_subscriber subscriber2 = corto_subscribe(CORTO_ON_UPDATE, "/myRoot", "sync/A")
                                        .contentType("text/json")
                                        .callback(OnNotify);
    corto_subscriber subscriber3 = corto_subscribe(CORTO_ON_DELETE, "/myRoot/sync", "//*")
                                        .contentType("text/json")
                                        .callback(OnDelete);

    //
    printf("\n");
    corto_int64Update(A, 124); //usleep(1000*100);
    corto_int64Update(A, 122); //usleep(1000*100);
    corto_int64Update(A, 1232); //usleep(1000*100);
    corto_int64Update(A, 12); //usleep(1000*100);

    corto_unsubscribe(subscriber, nullptr);
    // corto_release(Aea);
    // corto_release(Aeb);
    // corto_release(Aec);
    // corto_release(Aed);
    corto_delete(Ae);
    corto_release(Ad);
    corto_release(Ac);
    corto_release(Ab);
    corto_release(Aa);
    corto_release(A);

    corto_release(Be);
    corto_release(Bd);
    corto_release(Bc);
    corto_release(Bb);
    corto_release(Ba);
    corto_release(B);

    usleep(2000*2000);
    printf("\n\n");
    corto_resultIter it;
    printf("%s\n", "corto_select(\"myRoot\",\"mount//*\")");
    corto_select("myRoot", "mount//*").contentType("text/json").iter(&it);

    corto_resultIterForeach(it, r1) {
        printf("    Query returned '%s/%s' with value '%s'\n", r1.parent, r1.id, (corto_string)r1.value);
    }

    printf("\n\n");
    corto_resultIter it2;
    printf("%s\n", "corto_select(\"sync\",\"//*\")");
    corto_select("myRoot", "sync//*").contentType("text/json").iter(&it2);

    corto_resultIterForeach(it2, r2) {
        printf("    Query returned '%s/%s' with value '%s'\n", r2.parent, r2.id, (corto_string)r2.value);
    }

    // printf("%s\n", "corto_select(\"sync\",\"A//*\")");
    // corto_select("sync", "A//*").contentType("text/json").iter(&it);
    //
    // corto_resultIterForeach(it, r2) {
    //     printf("    Query returned '%s.%s' with value '%s'\n", r2.parent, r2.id, (corto_string)r2.value);
    // }
    //
    // printf("%s\n", "corto_select(\"sync\",\"/*\")");
    // corto_select("sync", "/*").contentType("text/json").iter(&it);
    //
    // corto_resultIterForeach(it, r3) {
    //     printf("    Query returned '%s.%s' with value '%s'\n", r3.parent, r3.id, (corto_string)r3.value);
    // }
    //
    // printf("%s\n", "corto_select(\"sync\",\"//*\")");
    // corto_select("sync", "//*").contentType("text/json").iter(&it);
    //
    // corto_resultIterForeach(it, r4) {
    //     printf("    Query returned '%s.%s' with value '%s'\n", r4.parent, r4.id, (corto_string)r4.value);
    // }

    test_assert(true);
/* $end */
}
