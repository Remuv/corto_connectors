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
    bool pass = true;

    corto_eventMask mask = CORTO_ON_UPDATE | CORTO_ON_SCOPE | CORTO_ON_TREE; //CORTO_ON_SELF

    corto_object mount1 = corto_voidCreateChild(root_o, "mount1");

    corto_object mount2 = corto_voidCreateChild(root_o, "mount2");

    clmdb_Connector connector1 = clmdb_ConnectorCreateChild(root_o,
                                    "lmdb1", mount1, mask,
                                    "/home/rfloresx/test.db", 0, 0664, 1UL*1024UL*1024UL*1024UL);

    clmdb_Connector connector2 = clmdb_ConnectorCreateChild(root_o,
                                    "lmdb2", mount2, mask,
                                    "/home/rfloresx/test.db", 0, 0664, 1UL*1024UL*1024UL*1024UL);


    //clmdb_ConnectorRegisterOnUpdate(child);

    std::string sub_fix = "test";
    for (int i = 0; i < 10; i++)
    {
        corto_attr attr;
        if (i%2 != 0)
        {
            attr = corto_setAttr(0);
        }
        std::string name = sub_fix+std::to_string(i);
        corto_object child = test_DataCreateChild(mount1, (char*)name.c_str(), (char*)name.c_str());
        if (i%2 == 0)
        {
            if (!corto_checkAttr(child, CORTO_ATTR_PERSISTENT))
            {
                corto_error("no Pass");
            }
        }
        else
        {
            if (corto_checkAttr(child, CORTO_ATTR_PERSISTENT))
            {
                corto_error("Does not Pass");
            }
        }
        if (i%2 != 0)
        {
            attr = corto_setAttr(attr);
        }
        CORTO_UNUSED(child);
    }
    
    for (int i = 0; i < 10; i++)
    {
        std::string name = sub_fix+std::to_string(i);
        corto_object child = corto_resolve(mount2, (char*)name.c_str());
        if (child == nullptr && i%2 == 0)
        {
            corto_error("Failed to resolve");
            pass = false;
        }
    }

    corto_delete(connector1);
    corto_delete(connector2);
    test_assert(pass);
/* $end */
}
