/* $CORTO_GENERATED
 *
 * MySuite.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <test.h>

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

    corto_resultIterForeach(it, r1) {
        printf("Query returned '%s' with value '%s'\n", r1.id, (corto_string)r1.value);
    }

    printf("%s\n", "corto_select(\"mount\",\"A//*\")");
    corto_select("mount", "A//*").contentType("text/json").iter(&it);

    corto_resultIterForeach(it, r2) {
        printf("Query returned '%s' with value '%s'\n", r2.id, (corto_string)r2.value);
    }

    printf("%s\n", "corto_select(\"mount\",\"/*\")");
    corto_select("mount", "/*").contentType("text/json").iter(&it);

    corto_resultIterForeach(it, r3) {
        printf("Query returned '%s' with value '%s'\n", r3.id, (corto_string)r3.value);
    }

    printf("%s\n", "corto_select(\"mount\",\"//A*\")");
    corto_select("mount", "//A*").contentType("text/json").iter(&it);

    corto_resultIterForeach(it, r4) {
        printf("Query returned '%s' with value '%s'\n", r4.id, (corto_string)r4.value);
    }

    printf("%s\n", "corto_select(\"mount\",\"//A*\")");
    corto_select("mount", "//*").contentType("text/json").iter(&it);

    corto_resultIterForeach(it, r5) {
        printf("Query returned '%s' with value '%s'\n", r5.id, (corto_string)r5.value);
    }

    /* << Insert implementation >> */
    test_assert(1);
/* $end */
}
