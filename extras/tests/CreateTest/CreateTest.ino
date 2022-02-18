#include <AUnit.h>
#include <Wappsto.h>


test(createNetwork) {
    WiFiClientSecure client;

    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, NO_LOGS);

    wappsto.connect();

    Network *myNetwork = wappsto.createNetwork("Basic Example", "");
    assertNotEqual(myNetwork, NULL);
    assertEqual(myNetwork->name, "Basic Example");

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":41385,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":41385,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createNetworkFailNotConnected) {
    WiFiClientSecure client;

    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, NO_LOGS);

    Network *myNetwork = wappsto.createNetwork("Basic Example", "");
    assertEqual(myNetwork, NULL);
}

test(createFirstConnect) {
    WiFiClientSecure client;

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

    ValueNumber_t numberParams = {  .name = "Test number",
                                    .type = "test type",
                                    .permission = READ_WRITE,
                                    .min = 0,
                                    .max = 1,
                                    .step = 1,
                                    .unit = "",
                                    .si_conversion = ""};


    Value *myNumberValue = myDevice->createValueNumber(&numberParams);
    assertNotEqual(myNumberValue, NULL);

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":22082,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":22083,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":22084,"method":"POST","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"c3b542b7-bb41-4f32-3648-5102aea6fca5","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":22085,"method":"GET","params":{"url":"/device/c3b542b7-bb41-4f32-3648-5102aea6fca5/value?this_name==Test number"}}{"jsonrpc":"2.0","id":22086,"method":"POST","params":{"url":"/device/c3b542b7-bb41-4f32-3648-5102aea6fca5/value","meta":{"fast":true},"data":{"name":"Test number","permission":"rw","type":"test type","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"be9251db-568c-43c6-20b2-70a3282a95d5","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":22087,"method":"GET","params":{"url":"/value/be9251db-568c-43c6-20b2-70a3282a95d5/state?this_type==Report"}}{"jsonrpc":"2.0","id":22088,"method":"POST","params":{"url":"/value/be9251db-568c-43c6-20b2-70a3282a95d5/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Report","meta":{"id":"58ac2384-aa7c-4f77-08ff-253175a91a74","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":22089,"method":"GET","params":{"url":"/value/be9251db-568c-43c6-20b2-70a3282a95d5/state?this_type==Control"}}{"jsonrpc":"2.0","id":22090,"method":"POST","params":{"url":"/value/be9251db-568c-43c6-20b2-70a3282a95d5/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"831649d1-7302-4671-b38e-4210805009cb","type":"state","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":22082,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":22083,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":22084,\"method\":\"POST\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"c3b542b7-bb41-4f32-3648-5102aea6fca5\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":22085,\"method\":\"GET\",\"params\":{\"url\":\"/device/c3b542b7-bb41-4f32-3648-5102aea6fca5/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":22086,\"method\":\"POST\",\"params\":{\"url\":\"/device/c3b542b7-bb41-4f32-3648-5102aea6fca5/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"rw\",\"type\":\"test type\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"be9251db-568c-43c6-20b2-70a3282a95d5\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":22087,\"method\":\"GET\",\"params\":{\"url\":\"/value/be9251db-568c-43c6-20b2-70a3282a95d5/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":22088,\"method\":\"POST\",\"params\":{\"url\":\"/value/be9251db-568c-43c6-20b2-70a3282a95d5/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Report\",\"meta\":{\"id\":\"58ac2384-aa7c-4f77-08ff-253175a91a74\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":22089,\"method\":\"GET\",\"params\":{\"url\":\"/value/be9251db-568c-43c6-20b2-70a3282a95d5/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":22090,\"method\":\"POST\",\"params\":{\"url\":\"/value/be9251db-568c-43c6-20b2-70a3282a95d5/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"831649d1-7302-4671-b38e-4210805009cb\",\"type\":\"state\",\"version\":\"2.0\"}}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createDevice) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");

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

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":45299,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":45300,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":45301,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":45299,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":45300,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":45301,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createValueNumber) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "0");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "0");

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

    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55641", myDevice->getUUID());

    ValueNumber_t numberParams = {  .name = "Test number",
                                    .type = "test type",
                                    .permission = READ_WRITE,
                                    .min = 0,
                                    .max = 1,
                                    .step = 1,
                                    .unit = "",
                                    .si_conversion = ""};


    Value *myNumberValue = myDevice->createValueNumber(&numberParams);
    assertNotEqual(myNumberValue, NULL);
    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55642", myNumberValue->getUUID());
    assertEqual("0", myNumberValue->getReportData());
    assertEqual("0", myNumberValue->getControlData());

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":24797,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":24798,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":24799,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":24800,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":24801,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"rw","type":"test type","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":24802,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":24803,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":24804,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":24805,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":24797,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":24798,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":24799,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":24800,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":24801,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"rw\",\"type\":\"test type\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":24802,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":24803,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":24804,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":24805,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createValueString) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "Test data Report");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "Test data Control");

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

    ValueString_t stringParams = {  .name = "Test String",
                                    .type = "test type",
                                    .permission = READ_WRITE,
                                    .max = 25,
                                    .encoding = ""};

    Value *myStringValue = myDevice->createValueString(&stringParams);
    assertNotEqual(myStringValue, NULL);
    assertEqual("Test data Report", myStringValue->getReportData());
    assertEqual("Test data Control", myStringValue->getControlData());

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":15383,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":15384,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":15385,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":15386,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test String"}}{"jsonrpc":"2.0","id":15387,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test String","permission":"rw","type":"test type","string":{"max":25,"encoding":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":15388,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":15389,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":15390,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":15391,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":15383,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":15384,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":15385,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":15386,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test String\"}}{\"jsonrpc\":\"2.0\",\"id\":15387,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test String\",\"permission\":\"rw\",\"type\":\"test type\",\"string\":{\"max\":25,\"encoding\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":15388,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":15389,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":15390,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":15391,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createValueBlob) {
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

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":63839,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":63840,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":63841,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":63842,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test Blob"}}{"jsonrpc":"2.0","id":63843,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test Blob","permission":"rw","type":"test type","blob":{"max":25,"encoding":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":63844,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":63845,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":63846,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":63847,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":63839,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":63840,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":63841,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":63842,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test Blob\"}}{\"jsonrpc\":\"2.0\",\"id\":63843,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test Blob\",\"permission\":\"rw\",\"type\":\"test type\",\"blob\":{\"max\":25,\"encoding\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":63844,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":63845,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":63846,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":63847,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createValueXml) {
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

    ValueXml_t xmlParams = {  .name = "Test XML",
                                    .type = "test type",
                                    .permission = READ_WRITE,
                                    .xml_namespace = "namespace",
                                    .xsd = "xsd"};


    Value *myXmlValue = myDevice->createValueXml(&xmlParams);
    assertNotEqual(myXmlValue, NULL);
    assertEqual("04FFEA", myXmlValue->getReportData());
    assertEqual("AEFF40", myXmlValue->getControlData());

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":60394,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":60395,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":60396,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":60397,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test XML"}}{"jsonrpc":"2.0","id":60398,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test XML","permission":"rw","type":"test type","xml":{"xsd":"xsd","namespace":"namespace"},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":60399,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":60400,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":60401,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":60402,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":60394,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":60395,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":60396,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":60397,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test XML\"}}{\"jsonrpc\":\"2.0\",\"id\":60398,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test XML\",\"permission\":\"rw\",\"type\":\"test type\",\"xml\":{\"xsd\":\"xsd\",\"namespace\":\"namespace\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":60399,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":60400,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":60401,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":60402,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createValueReadOnly) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "0");

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

    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55641", myDevice->getUUID());

    ValueNumber_t numberParams = {  .name = "Test number",
                                    .type = "test type",
                                    .permission = READ,
                                    .min = 0,
                                    .max = 1,
                                    .step = 1,
                                    .unit = "",
                                    .si_conversion = ""};


    Value *myNumberValue = myDevice->createValueNumber(&numberParams);
    assertNotEqual(myNumberValue, NULL);
    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55642", myNumberValue->getUUID());
    assertEqual("0", myNumberValue->getReportData());

    assertEqual("", myNumberValue->getControlData());

    bool result = myNumberValue->report(1);
    assertEqual(result, true);

    result = myNumberValue->control(1);
    assertEqual(result, false);

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":22037,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":22038,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":22039,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":22040,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":22041,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"r","type":"test type","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":22042,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":22043,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":22044,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"1","type":"Report"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":22037,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":22038,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":22039,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":22040,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":22041,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"r\",\"type\":\"test type\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":22042,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":22043,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":22044,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"1\",\"type\":\"Report\"}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createValueWriteOnly) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "0");

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

    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55641", myDevice->getUUID());

    ValueNumber_t numberParams = {  .name = "Test number",
                                    .type = "test type",
                                    .permission = WRITE,
                                    .min = 0,
                                    .max = 1,
                                    .step = 1,
                                    .unit = "",
                                    .si_conversion = ""};

    Value *myNumberValue = myDevice->createValueNumber(&numberParams);
    assertNotEqual(myNumberValue, NULL);
    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55642", myNumberValue->getUUID());
    assertEqual("0", myNumberValue->getControlData());

    assertEqual("", myNumberValue->getReportData());

    bool result = myNumberValue->control(1);
    assertEqual(result, true);

    result = myNumberValue->report(1);
    assertEqual(result, false);

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":42130,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":42131,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":42132,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":42133,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":42134,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":42135,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":42136,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}{"jsonrpc":"2.0","id":42137,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"1","type":"Control"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":42130,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":42131,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":42132,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":42133,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":42134,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":42135,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":42136,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}{\"jsonrpc\":\"2.0\",\"id\":42137,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"1\",\"type\":\"Control\"}}}";
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
