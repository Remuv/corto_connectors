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
    corto_string path = corto_envparse("$CORTO_TARGET/etc/corto/%s.%s/corto/admin",
        CORTO_VERSION_MAJOR,
        CORTO_VERSION_MINOR);
    corto_error(path);
    corto_dealloc(path);
    admin_serverCreateChild(root_o, "admin", 9000);

    corto_object mount1 = corto_voidCreateChild(root_o, "mount1");

    corto_object mount2 =  corto_voidCreateChild(root_o, "mount2");

    //_parent, _name, mount, mask, path, flags, mode,  map_size
    clmdb_Connector connector1 = clmdb_ConnectorCreateChild(root_o,
                                    "lmdb1", mount1, CORTO_ON_SCOPE | CORTO_ON_TREE,
                                    "/home/rfloresx/test.db", 0, 0664, 1UL*1024UL*1024UL*1024UL);

    clmdb_Connector connector2 = clmdb_ConnectorCreateChild(root_o,
                                    "lmdb2", mount2, CORTO_ON_SCOPE | CORTO_ON_TREE,
                                    "/home/rfloresx/test.db", 0, 0664, 1UL*1024UL*1024UL*1024UL);
    std::string sub_fix = "test_key_s";
    for (int i = 0; i < 10; i++)
    {
        std::string name = sub_fix+std::to_string(i);
        corto_object child = test_DataCreateChild(mount1, (char*)name.c_str(), (char*)name.c_str());
    }

    for (int i = 0; i < 10; i++)
    {
        std::string name = sub_fix+std::to_string(i);
        corto_object child = corto_resolve(mount2, (char*)name.c_str()); //test_DataCreateChild(mount, (char*)name.c_str(), (char*)name.c_str());
        if (child == nullptr)
        {
            corto_error("Failed to resolve");
        }
        else
        {
            corto_error("Pass");
        }
    }
    corto_delete(connector1);
    corto_delete(connector2);
    test_assert(true);
/* $end */
}
