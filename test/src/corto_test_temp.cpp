#include "corto_test_temp.h"

int corto_test_tempMain(int argc, char* argv[])
{
    corto_object mount = corto_voidCreateChild(NULL, "mount");
    //_parent, corto_string _name, corto_object mount, dds_DdsType type_1, corto_string topic
    dds_Connector connector = dds_ConnectorCreateChild(NULL, "net", mount, Dds_Publisher | Dds_Subscriber, "Net");

    datamodels_sockjs_Data data = datamodels_sockjs_DataCreateChild(mount, "data1", "Hello", "uuid", "val");

    for (int i = 0; i < 1000; i++)
    {
        std::string v = "Hello "+std::to_string(i);
        datamodels_sockjs_DataUpdate(data, "hello", "uuid", (char*)v.c_str());
    }
    dds_Connector_sendRequest(connector, "*", "", "");
    usleep(1000*1000);

    return 0;
}
