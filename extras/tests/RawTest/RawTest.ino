#include <AUnit.h>
#include <Wappsto.h>


test(createValueBlobRaw) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "04FFEA");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "AEFF40");

    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, NO_LOGS);

    wappsto.connect();

    Network *myNetwork = wappsto.createNetwork("Basic Example", "");
    assertNotEqual(myNetwork, NULL);

    DeviceDescription_t myDeviceDescription = {
        .name = "My Test Device",
        .product = "CreateTest",
        .manufacturer = "",
        .description = "description",
        .version = "1.0",
        .serial = "00001",
        .protocol = "Json-RPC",
        .communication = "WiFi",
    };
    Device *myDevice = myNetwork->createDevice(&myDeviceDescription);
    assertNotEqual(myDevice, NULL);

    ValueBlob_t blobParams = {  .name = "Test Blob",
                                    .type = "test type",
                                    .permission = READ_WRITE,
                                    .max = 25,
                                    .encoding = ""};


    Value *myBlobValue = myDevice->createValueBlob(&blobParams);
    assertNotEqual(myBlobValue, NULL);
    assertEqual("04FFEA", myBlobValue->getReportData());
    assertEqual("AEFF40", myBlobValue->getControlData());

    const char test_array[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ultricies arcu mi, dictum pulvinar nunc aliquam et. Mauris magna tortor, euismod ac porta ultrices, fringilla luctus turpis. Nunc dignissim hendrerit nulla. Quisque sit amet sapien sit amet nulla faucibus lobortis in sit amet lorem. Nunc fermentum metus non facilisis tempor. Aliquam cursus augue et massa dictum dapibus. Proin hendrerit turpis sit amet quam ullamcorper lacinia ut nec sem. In et gravida lectus, in congue velit non. ";
    myBlobValue->report(test_array);

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test Blob"}}{"jsonrpc":"2.0","id":57010,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test Blob","permission":"rw","type":"test type","blob":{"max":25,"encoding":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57011,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}{"jsonrpc":"2.0","id":57015,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ultricies arcu mi, dictum pulvinar nunc aliquam et. Mauris magna tortor, euismod ac porta ultrices, fringilla luctus turpis. Nunc dignissim hendrerit nulla. Quisque sit amet sapien sit amet nulla faucibus lobortis in sit amet lorem. Nunc fermentum metus non facilisis tempor. Aliquam cursus augue et massa dictum dapibus. Proin hendrerit turpis sit amet quam ullamcorper lacinia ut nec sem. In et gravida lectus, in congue velit non. ","type":"Report"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test Blob\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test Blob\",\"permission\":\"rw\",\"type\":\"test type\",\"blob\":{\"max\":25,\"encoding\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ultricies arcu mi, dictum pulvinar nunc aliquam et. Mauris magna tortor, euismod ac porta ultrices, fringilla luctus turpis. Nunc dignissim hendrerit nulla. Quisque sit amet sapien sit amet nulla faucibus lobortis in sit amet lorem. Nunc fermentum metus non facilisis tempor. Aliquam cursus augue et massa dictum dapibus. Proin hendrerit turpis sit amet quam ullamcorper lacinia ut nec sem. In et gravida lectus, in congue velit non. \",\"type\":\"Report\"}}}";
    assertEqual(client.testCompareTestData(test), true);
}


void setup() {
#if ! defined(EPROXY_DUINO)
    delay(1000);
#endif

    SERIAL_PORT_MONITOR.begin(115200);
    while(!SERIAL_PORT_MONITOR);
}

void loop() {
     aunit::TestRunner::run();
}
