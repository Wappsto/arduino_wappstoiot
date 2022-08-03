#include <AUnit.h>
#include <Wappsto.h>

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

static int refreshCounter = 0;

void refreshCallback(Value *value)
{
    value = value;
    refreshCounter++;
}

test(createValueNumberFullPeriod) {
    WiFiClientSecure client;

    refreshCounter = 0;

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
                                        .period = "2",
                                        .delta = "0"};


    Value *myNumberValue = myDevice->createValueNumber(&numberParams);
    assertNotEqual(myNumberValue, NULL);
    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55642", myNumberValue->getUUID());
    assertEqual("0", myNumberValue->getReportData());
    assertEqual("0", myNumberValue->getControlData());

    myNumberValue->onRefresh(&refreshCallback);

    int delay5sec = 5;
    while(delay5sec > 0) {
        wappsto.dataAvailable();
        delay(1000);
        Serial.print("Waiting period count: ");
        Serial.println(delay5sec);
        delay5sec--;
    }

    assertEqual(refreshCounter, 2);
    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"rw","type":"test type","period":"2","delta":"0","number":{"min":-2,"max":5,"step":1,"unit":"","ordered_mapping":false,"mapping":{"-2":"Map -2","-1":"Map -1","0":"Map zero","1":"Map 1","2":"Map 2","3":"Map 3","4":"Map 4"}},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"2\",\"delta\":\"0\",\"number\":{\"min\":-2,\"max\":5,\"step\":1,\"unit\":\"\",\"ordered_mapping\":false,\"mapping\":{\"-2\":\"Map -2\",\"-1\":\"Map -1\",\"0\":\"Map zero\",\"1\":\"Map 1\",\"2\":\"Map 2\",\"3\":\"Map 3\",\"4\":\"Map 4\"}},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createValueNumberFullIntDelta) {
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
                                        .delta = "50"};


    Value *myNumberValue = myDevice->createValueNumber(&numberParams);
    assertNotEqual(myNumberValue, NULL);
    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55642", myNumberValue->getUUID());
    assertEqual("0", myNumberValue->getReportData());
    assertEqual("0", myNumberValue->getControlData());

    myNumberValue->onRefresh(&refreshCallback);

    myNumberValue->report(5); // Blocked. 0->5 < 50

    myNumberValue->report(51); // Goes through. 0->51 > 50

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"rw","type":"test type","period":"0","delta":"50","number":{"min":-2,"max":5,"step":1,"unit":"","ordered_mapping":false,"mapping":{"-2":"Map -2","-1":"Map -1","0":"Map zero","1":"Map 1","2":"Map 2","3":"Map 3","4":"Map 4"}},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}{"jsonrpc":"2.0","id":57016,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"51","type":"Report"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"50\",\"number\":{\"min\":-2,\"max\":5,\"step\":1,\"unit\":\"\",\"ordered_mapping\":false,\"mapping\":{\"-2\":\"Map -2\",\"-1\":\"Map -1\",\"0\":\"Map zero\",\"1\":\"Map 1\",\"2\":\"Map 2\",\"3\":\"Map 3\",\"4\":\"Map 4\"}},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}{\"jsonrpc\":\"2.0\",\"id\":57016,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"51\",\"type\":\"Report\"}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(createValueNumberFullDoubleDelta) {
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

    ValueNumberFull_t numberParams = {  .name = "Test number",
                                        .type = "test type",
                                        .permission = READ_WRITE,
                                        .min = -20.4,
                                        .max = 200.32,
                                        .step = 0.1,
                                        .unit = "",
                                        .si_conversion = "",
                                        .ordered_map = false,
                                        .meaningful_zero = false,
                                        .mapping = NULL,
                                        .period = "0",
                                        .delta = "50.5"};


    Value *myNumberValue = myDevice->createValueNumber(&numberParams);
    assertNotEqual(myNumberValue, NULL);
    assertEqual("42b7bb41-bf32-4648-1102-aea6fca55642", myNumberValue->getUUID());
    assertEqual("0", myNumberValue->getReportData());
    assertEqual("0", myNumberValue->getControlData());

    myNumberValue->onRefresh(&refreshCallback);

    myNumberValue->report(50.4); // Blocked. 0->50.4 < 50.5

    myNumberValue->report(50.6); // Goes through. 0->50.6 > 50.5

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Test Device","manufacturer":"","product":"CreateTest","version":"1.0","serial":"00001","description":"description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number"}}{"jsonrpc":"2.0","id":57010,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642"}}{"jsonrpc":"2.0","id":57011,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Test number","permission":"rw","type":"test type","period":"0","delta":"50.5","number":{"min":-20.4,"max":200.32,"step":0.1,"unit":"","ordered_mapping":false},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}{"jsonrpc":"2.0","id":57016,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"50.60","type":"Report"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Test Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Test Device\",\"manufacturer\":\"\",\"product\":\"CreateTest\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Test number\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\"}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Test number\",\"permission\":\"rw\",\"type\":\"test type\",\"period\":\"0\",\"delta\":\"50.5\",\"number\":{\"min\":-20.4,\"max\":200.32,\"step\":0.1,\"unit\":\"\",\"ordered_mapping\":false},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}{\"jsonrpc\":\"2.0\",\"id\":57016,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"50.60\",\"type\":\"Report\"}}}";
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