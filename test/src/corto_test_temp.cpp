#include "corto_test_temp.h"

int corto_test_tempMain(int argc, char* argv[])
{
    corto_object mount = corto_voidCreateChild(NULL, "mount");
    corto_object sync_mount = corto_voidCreateChild(NULL, "sync_mount");
    //_parent, corto_string _name, corto_object mount, dds_DdsType type_1, corto_string topic
    dds_Connector net = dds_ConnectorCreateChild(NULL, "net", mount, Dds_Publisher, "Net");

    for(int i = 0; i < 100; i++)
    {
        std::string name = "Child"+std::to_string(i);
        std::string val = std::to_string(i);
        datamodels_sockjs_Data data = datamodels_sockjs_DataCreateChild(mount, (char*)name.c_str(), "Hello", "uuid", (char*)val.c_str());
        CORTO_UNUSED(data);
    }

    dds_Connector sync_net = dds_ConnectorCreateChild(NULL, "sync_net", sync_mount, Dds_Subscriber, "Net");

    dds_Connector_sendRequest(sync_net, "*", "REQUEST", "");
    usleep(1000*1000);
    /*
    corto_int16 ret;
    corto_iter iter = corto_select("sync_mount", "*").iter(&ret);

    corto_resultIterForeach(iter, e) {
        corto_trace("on sync_mount: found %s of type %s", e.name, e.type);
    }
    datamodels_sockjs_Data data = datamodels_sockjs_DataCreateChild(mount, "data1", "Hello", "uuid", "val");

    datamodels_sockjs_Data sn_data = (datamodels_sockjs_Data)corto_resolve(sync_mount, "data1");
    if(sn_data == NULL)
    {
        corto_trace("Error sync_mount does not resolve data1");
        return 0;
    }
    corto_trace("resolve for sync_mount/data1: %s,%s,%s", sn_data->type, sn_data->UUID, sn_data->VALUE );

    for (int i = 0; i < 10; i++)
    {
        std::string v = "Hello "+std::to_string(i);
        datamodels_sockjs_DataUpdate(data, "hello", "uuid", (char*)v.c_str());
        corto_trace("Update mount/data1 to %s,%s,%s", data->type, data->UUID, data->VALUE);
        usleep(1000*50);
        corto_trace("Sync sync_mount/data1: %s,%s,%s", sn_data->type, sn_data->UUID, sn_data->VALUE);
        usleep(1000*50);
    }
    corto_trace("Send Request from sync_net %s, %s, %s", "data1", "UPDATE", "{\"hello\",\"uuid\",\"Some-new-value\"}");
    //usleep(1000*1000);
    corto_trace("final value mount/data1: %s,%s,%s", data->type,data->UUID, data->VALUE );
    corto_trace("final value sync_mount/data1: %s,%s,%s", sn_data->type, sn_data->UUID, sn_data->VALUE );

    //dds_Connector_sendRequest(sync_net, "data1", "UPDATE", "{\"hello\",\"uuid\",\"Some-new-value\"}");
    */
    usleep(1000*2000);

    corto_trace("sync_mount");
    corto_delete(sync_mount);
    corto_trace("sync_net");
    corto_delete(sync_net);
    corto_trace("mount");
    corto_delete(mount);
    corto_trace("net");
    corto_delete(net);
    usleep(1000*1000);
    corto_trace("Exit main");
    return 0;
}
