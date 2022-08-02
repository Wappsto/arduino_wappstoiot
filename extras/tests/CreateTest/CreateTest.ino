
#include <AUnit.h>
#include <Wappsto.h>


test(createNetwork) {
    WiFiClientSecure client;

    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, NO_LOGS);

    wappsto.connect();

    Network *myNetwork = wappsto.createNetwork("Basic Example");
    assertNotEqual(myNetwork, NULL);
    assertEqual(myNetwork->name, "Basic Example");

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createNetworkWithDescription) {
    WiFiClientSecure client;

    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, NO_LOGS);

    wappsto.connect();

    Network *myNetwork = wappsto.createNetwork("Basic Example", "Basic description");
    assertNotEqual(myNetwork, NULL);
    assertEqual(myNetwork->name, "Basic Example");
    assertEqual(myNetwork->description, "Basic description");

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"Basic description","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"Basic description\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}";
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
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"POST","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/deaddead-dead-4ead-9ead-deaddeaddead/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/device/deaddead-dead-4ead-9ead-deaddeaddead/value"}}{"jsonrpc":"2.0","id":57011,"method":"POST","params":{"url":"/device/deaddead-dead-4ead-9ead-deaddeaddead/value","meta":{"fast":true},"data":{"name":"Test number","permission":"rw","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Report","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"POST\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/deaddead-dead-4ead-9ead-deaddeaddead/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/device/deaddead-dead-4ead-9ead-deaddeaddead/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"POST\",\"params\":{\"url\":\"/device/deaddead-dead-4ead-9ead-deaddeaddead/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Report\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}";
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
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}";
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
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"rw","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
    assertEqual(client.testCompareTestData(test), true);
}

const KEY_VALUE_t keyMap[] =  {   {"-2", "Map -2"},
                                  {"-1", "Map -1"},
                                  {"0", "Map zero"},
                                  {"1", "Map 1"},
                                  {"2", "Map 2"},
                                  {"3", "Map 3"},
                                  {"4", "Map 4"},
                              };

const NUMBER_MAPPING_t test_map  = { .size = 7,
                                     .map = keyMap,
                                    };


test(createValueNumberFull) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "0");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "0");

    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, WARNING);

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

    ValueNumberFull_t numberParams = {  .name = "Test number",
                                        .type = "test type",
                                        .permission = READ_WRITE,
                                        .min = -2,
                                        .max = 5,
                                        .step = 1,
                                        .unit = "",
                                        .si_conversion = "",
                                        .ordered_map = false,
                                        .meaningful_zero = false,
                                        .mapping = &test_map,
                                        .period = "0",
                                        .delta = "0"};


    Value *myNumberValue = myDevice->createValueNumber(&numberParams);
    assertNotEqual(myNumberValue, NULL);
    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55642", myNumberValue->getUUID());
    assertEqual("0", myNumberValue->getReportData());
    assertEqual("0", myNumberValue->getControlData());

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"rw","type":"test type","period":"0","delta":"0","number":{"min":-2,"max":5,"step":1,"unit":"","ordered_mapping":false,"mapping":{"-2":"Map -2","-1":"Map -1","0":"Map zero","1":"Map 1","2":"Map 2","3":"Map 3","4":"Map 4"}},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":-2,\"max\":5,\"step\":1,\"unit\":\"\",\"ordered_mapping\":false,\"mapping\":{\"-2\":\"Map -2\",\"-1\":\"Map -1\",\"0\":\"Map zero\",\"1\":\"Map 1\",\"2\":\"Map 2\",\"3\":\"Map 3\",\"4\":\"Map 4\"}},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
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
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test String"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test String","permission":"rw","type":"test type","period":"0","delta":"0","string":{"max":25,"encoding":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test String\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test String\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"string\":{\"max\":25,\"encoding\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
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
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test Blob"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test Blob","permission":"rw","type":"test type","period":"0","delta":"0","blob":{"max":25,"encoding":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test Blob\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test Blob\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"blob\":{\"max\":25,\"encoding\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
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
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test XML"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test XML","permission":"rw","type":"test type","period":"0","delta":"0","xml":{"xsd":"xsd","namespace":"namespace"},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test XML\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test XML\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"xml\":{\"xsd\":\"xsd\",\"namespace\":\"namespace\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
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
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"r","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"1","type":"Report"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"r\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"1\",\"type\":\"Report\"}}}";
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
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}{"jsonrpc":"2.0","id":57014,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"1","type":"Control"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"1\",\"type\":\"Control\"}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createManyDevices) {
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
    int counter = 0;
    Device *myDevice;
    for(int i=0; i<10; i++) {
        myDevice = myNetwork->createDevice(&myDeviceDescription);
        if(myDevice) {
            counter++;
        } else {
            break;
        }
    }
    assertEqual(counter, 5);

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"POST","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57010,"method":"POST","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57011,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57012,"method":"POST","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57014,"method":"POST","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57016,"method":"POST","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"device","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"POST\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"POST\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"POST\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"POST\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57016,\"method\":\"POST\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"device\",\"version\":\"2.0\"}}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createManyValues) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");

    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, VERBOSE);

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

    int counter = 0;
    Value *myValue;
    for(int i=0; i<15; i++) {
        myValue = myDevice->createValueNumber(&numberParams);
        if(myValue) {
            counter++;
        } else {
            break;
        }
    }
    assertEqual(counter, 10);

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57011,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57013,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57016,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57017,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57018,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57019,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57020,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57021,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57022,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57023,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57024,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57025,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57026,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57027,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57028,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57029,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57030,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57031,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57032,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57033,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57034,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57035,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57036,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57037,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57038,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57039,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57040,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57041,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57042,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57043,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57044,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57045,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57046,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57047,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57048,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57049,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57050,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57051,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57052,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57053,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}{"jsonrpc":"2.0","id":57054,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57055,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value"}}{"jsonrpc":"2.0","id":57056,"method":"POST","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value","meta":{"fast":true},"data":{"name":"Test number","permission":"w","type":"test type","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57057,"method":"GET","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control"}}{"jsonrpc":"2.0","id":57058,"method":"POST","params":{"url":"/value/deaddead-dead-4ead-9ead-deaddeaddead/state","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"","type":"Control","meta":{"id":"deaddead-dead-4ead-9ead-deaddeaddead","type":"state","version":"2.0"}}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57016,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57017,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57018,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57019,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57020,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57021,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57022,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57023,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57024,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57025,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57026,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57027,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57028,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57029,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57030,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57031,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57032,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57033,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57034,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57035,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57036,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57037,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57038,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57039,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57040,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57041,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57042,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57043,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57044,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57045,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57046,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57047,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57048,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57049,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57050,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57051,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57052,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57053,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57054,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57055,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\"}}{\"jsonrpc\":\"2.0\",\"id\":57056,\"method\":\"POST\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"w\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57057,\"method\":\"GET\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57058,\"method\":\"POST\",\"params\":{\"url\":\"/value/deaddead-dead-4ead-9ead-deaddeaddead/state\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"\",\"type\":\"Control\",\"meta\":{\"id\":\"deaddead-dead-4ead-9ead-deaddeaddead\",\"type\":\"state\",\"version\":\"2.0\"}}}}";
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
