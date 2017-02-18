/* $CORTO_GENERATED
 *
 * MySuite.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <include/test.h>

/* $header() */
#define corto_resultForeach(iter, elem) \
    corto_result *elem;\
    while(corto_iterHasNext(&iter) ? elem = (corto_result*)(corto_word)corto_iterNext(&iter), TRUE : FALSE)

#define corto_sampleIterForeach(iter, elem) \
    corto_sample *elem;\
    while(corto_iterHasNext(&iter) ? elem = (corto_sample*)(corto_word)corto_iterNext(&iter), TRUE : FALSE)

corto_object createChildFromContent (
    corto_object _parent,
    corto_string _id,
    corto_type   type,
    corto_string contentType,
    corto_string fmt
)
{
    corto_object o = corto_declareChild(_parent, _id, type);
    if (!corto_checkState(o, CORTO_DEFINED)) {
        if (corto_fromcontent(o, contentType, fmt))
        {
            printf("ERRRRRRRSSSSSS\n");
        }

        if (corto_define(o))
        {
            printf("ERRR\n");
        }
    }
    else
    {
        printf("EEERRRRRR\n");
    }
    return o;
}

/* $end */

corto_void _test_MySuite_aTest(
    test_MySuite this)
{
/* $begin(test/MySuite/aTest) */
    //
    corto_time t = {10000000, 100000};
    test_setTimeout(&t);
    printf("==========TEST===============\n");
    corto_object mount = corto_voidCreateChild(root_o, "mount");

    corto_string config = "{\
        \"mount\":\"/mount\",\
        \"mask\":\"ON_TREE\",\
        \"dbname\":\"test_historian_db\",\
        \"user\":\"\",\
        \"password\":\"\",\
        \"hostaddr\":\"localhost\",\
        \"port\": 27017,\
        \"update_rate\": 0\
    }";

    //corto_fromcontent(corto_object o, corto_string contentType, corto_string fmt, ...)
    mongo_Connector mongo = mongo_Connector(createChildFromContent(root_o,
                                             "mm",
                                             corto_type(mongo_Historian_o),
                                             "text/json",
                                             config));
    /* << Insert implementation >> */
    // corto_string json = corto_contentof(NULL, "text/json", mongo);
    // printf("data: %s \n\n\n", json);
    // //corto_dealloc(json);
    //
    // corto_float32 *A = corto_float32CreateChild(mount, "A", 0);

    // test_Vec2 v0 = test_Vec2CreateChild(A, "v0", 0, 0);
    // for (int i = 1; i < 10; i++)
    // {
    //     usleep(1000*10);
    //     corto_float32Update(A, i);
    //     test_Vec2Update(v0, i, i*10);
    // }
    // //printf("\n")
    // corto_release(v0);
    // corto_release(A);

    // {
    //     corto_resultIter it;
    //     printf("%s\n", "corto_select(\"mount\",\"//A\")");
    //     corto_select("mount", "//A")
    //                 .fromSample(0)
    //                 .toNow()
    //                 .contentType("text/json")
    //                 .iter(&it);
    //     //
    //     corto_resultForeach(it, r1) {
    //         printf("Query returned '%s': \n", r1->id);
    //         corto_sampleIterForeach(r1->history, sample) {
    //             corto_time timestamp = sample->timestamp;
    //             corto_string value = (corto_string)sample->value;
    //             printf("%lis, %lins, value: %s\n",timestamp.sec, timestamp.nanosec ,value);
    //         }
    //     }
    // }

    {
        corto_resultIter it;
        printf("%s\n", "corto_select(\"mount\",\"//*\")");
        corto_select("mount", "//*")
                    .fromSample(0)
                    .toNow()
                    .contentType("text/json")
                    .iter(&it);
        //

        corto_resultForeach(it, r1)
        {
            printf("Query returned '%s': \n", r1->id);
            corto_sampleIterForeach(r1->history, sample) {
                corto_time timestamp = sample->timestamp;
                corto_string value = (corto_string)sample->value;
                printf("%lis, %lins, value: %s\n",timestamp.sec, timestamp.nanosec ,value);
            }
        }
    }
    printf("%i\n",corto_countof(mongo));
    corto_delete(mongo);
    printf("%i\n",corto_countof(mongo));
    test_assert(1);

/* $end */
}

corto_void _test_MySuite_testHistorical(
    test_MySuite this)
{
/* $begin(test/MySuite/testHistorical) */
    corto_time t = {10000000, 100000};
    test_setTimeout(&t);

    printf("TEST_HISTORIAN \n");

    corto_object mount = corto_voidCreateChild(root_o, "mount");
    mongo_Historian mongo = mongo_HistorianCreateChild(
        root_o,
        "mm",
        mount,
        CORTO_ON_TREE,
        "test_historian_db",
        "",
        "",
        "localhost",
        27017,
        0
    );

    corto_string json = corto_contentof(NULL, "text/json", mongo);
    printf("%s \n", json);

    //CORTO_UNUSED(mongo);
    corto_float32 *A = corto_float32CreateChild(mount, "A", 0);
    test_Vec2 v0 = test_Vec2CreateChild(A, "v0", 0, 0);
    for (int i = 1; i < 10; i++)
    {
        usleep(1000*100);
        corto_float32Update(A, i);
        test_Vec2Update(v0, i, i*10);
    }
    //printf("\n")
    corto_release(v0);
    corto_release(A);

    {
        corto_resultIter it;
        printf("%s\n", "corto_select(\"mount\",\"//A\")");
        corto_select("mount", "//A")
                    .fromSample(0)
                    .toNow()
                    .contentType("text/json")
                    .iter(&it);
        //
        corto_resultForeach(it, r1) {
            printf("Query returned '%s':\n", r1->id);
            corto_sampleIterForeach(r1->history, sample) {
                corto_time timestamp = sample->timestamp;
                corto_string value = (corto_string)sample->value;
                printf("%lis, %lins, value: %s\n",timestamp.sec, timestamp.nanosec ,value);
            }
        }
    }

    {
        corto_resultIter it;
        printf("%s\n", "corto_select(\"mount\",\"//*\")");
        corto_select("mount", "//*")
                    .fromSample(0)
                    .toNow()
                    .contentType("text/json")
                    .iter(&it);
        //
        corto_resultForeach(it, r1) {
            printf("Query returned '%s':\n", r1->id);
            corto_sampleIterForeach(r1->history, sample) {
                corto_time timestamp = sample->timestamp;
                corto_string value = (corto_string)sample->value;
                printf("%lis, %lins, value: %s\n",timestamp.sec, timestamp.nanosec ,value);
            }
        }
    }
    printf("Refounct: %i\n", corto_countof(mongo));
    corto_delete(mongo);
    test_assert(1);
/* $end */
}

corto_void _test_MySuite_testSomething(
    test_MySuite this)
{
/* $begin(test/MySuite/testSomething) */
    corto_object mount = corto_voidCreateChild(root_o, "mount");

    mongo_Connector mongo = mongo_ConnectorCreateChild(
        root_o,
        "mm",
        mount,
        CORTO_ON_TREE,
        "test_db",
        "",
        "",
        "localhost",
        27017
    );
    CORTO_UNUSED(mongo);
    corto_float32 *A = corto_float32CreateChild(mount, "A", 0);
     corto_float32 *B = corto_float32CreateChild(A, "B", 0);
     corto_float32 *C = corto_float32CreateChild(A, "C", 0);
    corto_float32 *D = corto_float32CreateChild(mount, "D", 0);
     corto_float32 *E = corto_float32CreateChild(D, "E", 0);

    test_Vec2 v0 = test_Vec2CreateChild(B, "v0", 1, 2);
    test_Vec2 v1 = test_Vec2CreateChild(B, "v1", 3, 4);
    test_Vec2 v2 = test_Vec2CreateChild(B, "v2", 5, 6);
    test_Vec2 v3 = test_Vec2CreateChild(B, "v3", 7, 8);

    test_Vec2 v = test_Vec2Create(34,52);
    test_Data d0 =  test_DataCreateChild(A, "v0", "v0", 1, .1f, .01, v);
    test_Data d1 =  test_DataCreateChild(A, "v1", "v1", 2, .2f, .02, v);
    test_Data d2 =  test_DataCreateChild(A, "v2", "v2", 3, .3f, .03, v);
    test_Data d3 =  test_DataCreateChild(A, "v3", "v3", 4, .4f, .04, v);


    test_Test data = test_TestCreate(1);
    test_Data complex = test_ObjectPtrCreateChild(mount, "complex", 1, data);

    corto_release(v);
    corto_release(A);
    corto_release(B);
    corto_release(C);
    corto_release(D);
    corto_release(E);
    corto_release(v0);
    corto_release(v1);
    corto_release(v2);
    corto_release(v3);
    corto_release(d0);
    corto_release(d1);
    corto_release(d2);
    corto_release(d3);

    corto_resultIter it;
    printf("%s\n", "corto_select(\"mount\",\"//A\")");
    corto_select("mount", "//A").contentType("text/json").iter(&it);

    corto_resultForeach(it, r1) {
        printf("Query returned '%s' with value '%s'\n", r1->id, (corto_string)r1->value);
    }

    printf("%s\n", "corto_select(\"mount\",\"A//*\")");
    corto_select("mount", "A//*").contentType("text/json").iter(&it);

    corto_resultForeach(it, r2) {
        printf("Query returned '%s' with value '%s'\n", r2->id, (corto_string)r2->value);
    }

    printf("%s\n", "corto_select(\"mount\",\"/*\")");
    corto_select("mount", "/*").contentType("text/json").iter(&it);

    corto_resultForeach(it, r3) {
        printf("Query returned '%s' with value '%s'\n", r3->id, (corto_string)r3->value);
    }

    printf("%s\n", "corto_select(\"mount\",\"//A*\")");
    corto_select("mount", "//A*").contentType("text/json").iter(&it);

    corto_resultForeach(it, r4) {
        printf("Query returned '%s' with value '%s'\n", r4->id, (corto_string)r4->value);
    }

    printf("%s\n", "corto_select(\"mount\",\"//A*\")");
    corto_select("mount", "//*").contentType("text/json").iter(&it);

    corto_resultForeach(it, r5) {
        printf("Query returned '%s' with value '%s'\n", r5->id, (corto_string)r5->value);
    }

    {
        printf("%s\n", "corto_select(\"mount\",\"//*\")");
        corto_select("mount", "//*").contentType("text/json").iter(&it);

        corto_resultForeach(it, r6) {
            printf("Query returned '%s/%s' with value '%s'\n", r6->parent, r6->id, (corto_string)r6->value);
        }
    }

    corto_object t = corto_resolve(mount, "A");
    corto_delete(t);
    {
        printf("%s\n", "corto_select(\"mount\",\"//*\")");
        corto_select("mount", "//*").contentType("text/json").iter(&it);

        corto_resultForeach(it, r6) {
            printf("Query returned '%s/%s' with value '%s'\n", r6->parent, r6->id, (corto_string)r6->value);
        }
    }

    printf("%i\n", corto_countof(mongo));

    corto_delete(mongo);
    test_assert(1);
/* $end */
}
