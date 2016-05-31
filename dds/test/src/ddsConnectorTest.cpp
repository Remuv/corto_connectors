/* $CORTO_GENERATED
 *
 * ddsConnectorTest.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <test.h>

corto_void _test_ddsConnectorTest_StartTest(
    test_ddsConnectorTest _this)
{
/* $begin(test/ddsConnectorTest/StartTest) */
//*
    corto_object mount = corto_voidCreateChild(NULL, "mount");
    //_parent, corto_string _name, corto_object mount, dds_DdsType type_1, corto_string topic
    dds_Connector connector = dds_ConnectorCreateChild(NULL, "net", mount, Dds_Publisher | Dds_Subscriber, "Net");
    dds_Connector_Init(connector);
    test_Data data = test_DataCreateChild(mount, "data1", "Hello");

    for (int i = 0; i < 1000; i++)
    {
        std::string v = "Hello "+std::to_string(i);
        test_DataUpdate(data, (char*)v.c_str());
    }
    test_assert(true);
//*/
/* $end */
}
