/* $CORTO_GENERATED
 *
 * clmdbTest.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <test.h>

/* $header() */
#include <string>



/* $end */

corto_void _test_clmdbTest_StartTest(
    test_clmdbTest _this)
{
/* $begin(test/clmdbTest/StartTest) */

    corto_object myRoot = corto_voidCreateChild(root_o, "MyRoot");
    corto_object myObjects = corto_voidCreateChild(myRoot, "Objects");

    corto_object mount = corto_voidCreateChild(myObjects, "mount");
    clmdb_Connector connector1 = clmdb_ConnectorCreateChild(
        root_o,
        "lmdb",
        mount,
        CORTO_ON_TREE,
        "/home/rfloresx/test.db",
        0,
        0664,
        1UL*1024UL*1024UL*1024UL
    );
    corto_object mount2 = corto_voidCreateChild(myObjects, "mount2");
    clmdb_Connector connector2 = clmdb_ConnectorCreateChild(
        root_o,
        "lmdb2",
        mount2,
        CORTO_ON_TREE,
        "/home/rfloresx/test2.db",
        0,
        0664,
        1UL*1024UL*1024UL*1024UL
    );

    corto_float32 *A = corto_float32CreateChild(mount, "A", 1);
    corto_float32 *B = corto_float32CreateChild(A, "B", 2);
    corto_float32 *C = corto_float32CreateChild(A, "C", 3);
    corto_float32 *D = corto_float32CreateChild(mount, "D", 4);
    corto_float32 *E = corto_float32CreateChild(D, "E", 5);

    corto_float32 *F = corto_float32CreateChild(mount2, "F", 6);
    corto_float32 *G = corto_float32CreateChild(F, "G", 7);
    corto_float32 *H = corto_float32CreateChild(F, "H", 8);
    corto_float32 *I = corto_float32CreateChild(mount2, "I", 9);
    corto_float32 *J = corto_float32CreateChild(I, "J", 10);

    corto_resultIter it;
    //printf("%s\n", "corto_select(\"/MyRoot/Objects\",\"//*\")");
    corto_select("/MyRoot/Objects", "mount/A/*").contentType("text/json").iter(&it);

    corto_resultIterForeach(it, r1) {
        printf("Query returned '%s' with value '%s'\n", r1.id, (corto_string)r1.value);
    }

    corto_release(A);
    corto_release(B);
    corto_release(C);
    corto_release(D);
    corto_release(E);
    corto_release(F);
    corto_release(G);
    corto_release(H);
    corto_release(I);
    corto_release(J);

    //
    // test_Vec2 v0 = test_Vec2CreateChild(B, "v0", 1, 6);
    // test_Vec2 v1 = test_Vec2CreateChild(B, "v1", 3, 7);
    // test_Vec2 v2 = test_Vec2CreateChild(B, "v2", 5, 8);
    // test_Vec2 v3 = test_Vec2CreateChild(B, "v3", 7, 9);
    //
    // test_Vec2 v = test_Vec2Create(34,52);
    // test_Data d0 =  test_DataCreateChild(A, "d0", "v0", 1, .1f, .01, v);
    // test_Data d1 =  test_DataCreateChild(A, "d1", "v1", 2, .2f, .02, v);
    // test_Data d2 =  test_DataCreateChild(A, "d2", "v2", 3, .3f, .03, v);
    // test_Data d3 =  test_DataCreateChild(A, "d3", "v3", 4, .4f, .04, v);
    //
    // corto_release(v);
    // corto_release(A);
    // corto_release(B);
    // corto_release(C);
    // corto_release(D);
    // corto_release(E);
    // corto_delete(v0);
    // corto_delete(v1);
    // corto_release(v2);
    // corto_release(v3);
    // corto_release(d0);
    // corto_release(d1);
    // corto_release(d2);
    // corto_release(d3);

    // corto_resultIter it;
    // printf("%s\n", "corto_select(\"mount\",\"//*\")");
    // corto_select("mount", "//*").contentType("text/json").iter(&it);
    //
    // corto_resultIterForeach(it, r1) {
    //     printf("Query returned '%s' with value '%s'\n", r1.id, (corto_string)r1.value);
    // }

    // printf("%s\n", "corto_select(\"mount\",\"A//*\")");
    // corto_select("mount", "A//*").contentType("text/json").iter(&it);
    //
    // corto_resultIterForeach(it, r2) {
    //     printf("Query returned '%s' with value '%s'\n", r2.id, (corto_string)r2.value);
    // }
    //
    // printf("%s\n", "corto_select(\"mount\",\"/*\")");
    // corto_select("mount", "/*").contentType("text/json").iter(&it);
    //
    // corto_resultIterForeach(it, r3) {
    //     printf("Query returned '%s' with value '%s'\n", r3.id, (corto_string)r3.value);
    // }
    //
    // printf("%s\n", "corto_select(\"mount\",\"//A*\")");
    // corto_select("mount", "//A*").contentType("text/json").iter(&it);
    //
    // corto_resultIterForeach(it, r4) {
    //     printf("Query returned '%s' with value '%s'\n", r4.id, (corto_string)r4.value);
    // }
    //
    // printf("%s\n", "corto_select(\"mount/A\",\"//*\")");
    // corto_select("mount/A", "//*").contentType("text/json").iter(&it);
    //
    // corto_resultIterForeach(it, r5) {
    //     printf("Query returned '%s' with value '%s'\n", r5.id, (corto_string)r5.value);
    // }
    //corto_delete(connector2);
    test_assert(true);
/* $end */
}
