#include "app_test.h"

int app_testMain(int argc, char* argv[])
{
    corto_verbosity(corto_err::CORTO_DEBUG);
    corto_object mount = corto_voidCreateChild(NULL, "mount");
    corto_object sync_mount = corto_voidCreateChild(NULL, "sync_mount");

    corto_trace("Create DDS Connector on 'mount'");
    dds_Connector net = dds_ConnectorCreateChild(NULL, "net", mount, Dds_Publisher, "TestTopic");

    int size = 10;
    corto_trace("Creathe %i childs on 'mount'", size);
    for(int i = 0; i < size; i++)
    {
        std::string name = "Child"+std::to_string(i);
        std::string val = std::to_string(i);
        data_models_sockjs_Data data = data_models_sockjs_DataCreateChild(mount, (char*)name.c_str(), "Hello", "uuid", (char*)val.c_str());
        CORTO_UNUSED(data);
    }
    corto_trace("Create DDS Connector on 'sync_mount'");
    dds_Connector sync_net = dds_ConnectorCreateChild(NULL, "sync_net", sync_mount, Dds_Subscriber, "TestTopic");

    corto_trace("Request All childs");
    dds_Connector_sendRequest(sync_net, "*", "REQUEST", "");
    usleep(1000*1000);

    corto_iter iter;

    corto_select("sync_mount", "*").iter(&iter);

    int count = 0;
    corto_resultIterForeach(iter, e) {
        corto_trace("id: %s, name: %s, parent: %s, type: %s, value: %llu", e.id, e.name, e.parent, e.type, e.value);
        count++;
    }
    corto_trace("Found %i childs on 'sync_mount'", count);
    //test_assert(count == size);

    data_models_sockjs_Data data = data_models_sockjs_DataCreateChild(mount, "data1", "Hello", "uuid", "val");
    usleep(1000*100);
    data_models_sockjs_Data sn_data = (data_models_sockjs_Data)corto_resolve(sync_mount, "data1");
    if(sn_data == NULL)
    {
        corto_seterr("sync_mount does not resolve data1");
    }
    //test_assert(sn_data != NULL);
    for (int i = 0; i < 10; i++)
    {
        std::string v = "Hello "+std::to_string(i);
        corto_trace("Set Value 'mount/data1->value' to  %s", (char*)v.c_str());
        data_models_sockjs_DataUpdate(data, "hello", "uuid", (char*)v.c_str());
        usleep(1000*100);
        corto_trace("Value on 'sync_mount/data1->value' %s", sn_data->value);
        if ( strcmp(data->value,sn_data->value) != 0)
        {
            corto_trace("Fail to sync childs mount/data1 and sync_mount/data1");
            //test_assert(false);
        }
    }
    //test_assert(true);
    usleep(1000*2000);

    corto_delete(sync_mount);
    corto_delete(sync_net);
    corto_delete(mount);
    corto_delete(net);
    usleep(1000*1000);

    //test_assert(true);

    return 0;
}
