#include <AUnit.h>
#include <Wappsto.h>

static bool receivedControl = false;
static bool receivedRefresh = false;


void controlStringCallback(Value *value, String data, String timestamp)
{
    (void)value;
    (void)timestamp;
    if(strcmp("Correct test string", data.c_str()) == 0) {
        receivedControl = true;
    } else {
        receivedControl = false;
    }
}

void controlNumberCallback(Value *value, double data, String timestamp)
{
    (void)value;
    (void)timestamp;
    if(data == 1) {
        receivedControl = true;
    } else {
        receivedControl = false;
    }
}

void refreshCallback(Value *value)
{
    (void)value;
    receivedRefresh = true;
}

test(callbackNumberTest) {
    WiFiClientSecure client;

    // Test setup
    receivedRefresh = false;
    receivedControl = false;
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "0");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "0");

    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55645");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55646", "0.987");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55647", "0.987");


    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, NO_LOGS);

    wappsto.connect();

    Network *myNetwork = wappsto.createNetwork("Basic Example", "");
    assertNotEqual(myNetwork, NULL);
    DeviceDescription_t myDeviceDescription = {
        .name = "My Device",
        .product = "ESP32 example",
        .manufacturer = "",
        .description = "ESP32 Example description",
        .version = "1.0",
        .serial = "00001",
        .protocol = "Json-RPC",
        .communication = "WiFi",
    };
    Device *myDevice = myNetwork->createDevice(&myDeviceDescription);
    assertNotEqual(myDevice, NULL);

    ValueNumber_t myIntParameters = {   .name = "Int",
                                        .type = "bool",
                                        .permission = READ_WRITE,
                                        .min = 0,
                                        .max = 1,
                                        .step = 1,
                                        .unit = "",
                                        .si_conversion = ""};

    ValueNumber_t myDoubleParam = {   .name = "float",
                                        .type = "number",
                                        .permission = READ_WRITE,
                                        .min = -180,
                                        .max = 180,
                                        .step = 0.000001,
                                        .unit = "",
                                        .si_conversion = ""};


    Value *myIntValue = myDevice->createValueNumber(&myIntParameters);
    myIntValue->onControl(&controlNumberCallback);
    myIntValue->onRefresh(&refreshCallback);

    Value *myDoubleValue = myDevice->createValueNumber(&myDoubleParam);

    // Test refresh
    client.testRefresh("42b7bb41-bf32-4648-1102-aea6fca55643", "/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641/value/42b7bb41-bf32-4648-1102-aea6fca55642/state/");
    wappsto.dataAvailable();
    assertEqual(receivedRefresh, true);

    // Test control
    client.testControl("42b7bb41-bf32-4648-1102-aea6fca55644", "/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641/value/42b7bb41-bf32-4648-1102-aea6fca55642/state/", "1");
    wappsto.dataAvailable();
    assertEqual(receivedControl, true);

    client.testControl("42b7bb41-bf32-4648-1102-aea6fca55644", "/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641/value/42b7bb41-bf32-4648-1102-aea6fca55642/state/", "2");
    wappsto.dataAvailable();
    assertEqual(receivedControl, false);

    double myLatitude = 57.019660;
    int testInt = 123;

    // Test report
    myIntValue->report(testInt);
    assertEqual("123", myIntValue->getReportData());

    Serial.println("Report step size 1 - max .00");
    myIntValue->report(myLatitude);
    assertEqual("57.02", myIntValue->getReportData());

    Serial.println("Report step size 0.000001");
    myDoubleValue->report(myLatitude);
    assertEqual("57.019660", myDoubleValue->getReportData());


    // Test control
    myIntValue->control(testInt);
    assertEqual("123", myIntValue->getControlData());

    Serial.println("Control step size 1 - max .00");
    myIntValue->control(myLatitude);
    assertEqual("57.02", myIntValue->getControlData());

    Serial.println("Control  step size 0.000001");
    myDoubleValue->control(myLatitude);
    assertEqual("57.019660", myDoubleValue->getControlData());

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Device","manufacturer":"","product":"ESP32 example","version":"1.0","serial":"00001","description":"ESP32 Example description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Int"}}{"jsonrpc":"2.0","id":57010,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"Int","permission":"rw","type":"bool","period":"0","delta":"0","number":{"min":0,"max":1,"step":1,"unit":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57011,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}{"jsonrpc":"2.0","id":57015,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==float"}}{"jsonrpc":"2.0","id":57016,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55645","meta":{"fast":true},"data":{"name":"float","permission":"rw","type":"number","period":"0","delta":"0","number":{"min":-180,"max":180,"step":1e-6,"unit":""},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55645","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57017,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55645/state?this_type==Report"}}{"jsonrpc":"2.0","id":57018,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55646"}}{"jsonrpc":"2.0","id":57019,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55645/state?this_type==Control"}}{"jsonrpc":"2.0","id":57020,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55647"}}{"jsonrpc":"2.0","id":"TestRefresh","result":true}{"jsonrpc":"2.0","id":"TestControl","result":true}{"jsonrpc":"2.0","id":"TestControl","result":true}{"jsonrpc":"2.0","id":57021,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"123","type":"Report"}}}{"jsonrpc":"2.0","id":57022,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"57.02","type":"Report"}}}{"jsonrpc":"2.0","id":57023,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55646","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"57.019660","type":"Report"}}}{"jsonrpc":"2.0","id":57024,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"123","type":"Control"}}}{"jsonrpc":"2.0","id":57025,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"57.02","type":"Control"}}}{"jsonrpc":"2.0","id":57026,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55647","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"57.019660","type":"Control"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Device\",\"manufacturer\":\"\",\"product\":\"ESP32 example\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"ESP32 Example description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==Int\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Int\",\"permission\":\"rw\",\"type\":\"bool\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":0,\"max\":1,\"step\":1,\"unit\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==float\"}}{\"jsonrpc\":\"2.0\",\"id\":57016,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55645\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"float\",\"permission\":\"rw\",\"type\":\"number\",\"period\":\"0\",\"delta\":\"0\",\"number\":{\"min\":-180,\"max\":180,\"step\":1e-6,\"unit\":\"\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55645\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57017,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55645/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57018,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55646\"}}{\"jsonrpc\":\"2.0\",\"id\":57019,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55645/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57020,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55647\"}}{\"jsonrpc\":\"2.0\",\"id\":\"TestRefresh\",\"result\":true}{\"jsonrpc\":\"2.0\",\"id\":\"TestControl\",\"result\":true}{\"jsonrpc\":\"2.0\",\"id\":\"TestControl\",\"result\":true}{\"jsonrpc\":\"2.0\",\"id\":57021,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"123\",\"type\":\"Report\"}}}{\"jsonrpc\":\"2.0\",\"id\":57022,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"57.02\",\"type\":\"Report\"}}}{\"jsonrpc\":\"2.0\",\"id\":57023,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55646\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"57.019660\",\"type\":\"Report\"}}}{\"jsonrpc\":\"2.0\",\"id\":57024,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"123\",\"type\":\"Control\"}}}{\"jsonrpc\":\"2.0\",\"id\":57025,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"57.02\",\"type\":\"Control\"}}}{\"jsonrpc\":\"2.0\",\"id\":57026,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55647\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"57.019660\",\"type\":\"Control\"}}}";
    assertEqual(client.testCompareTestData(test), true);
}

test(callbackStringTest) {
    WiFiClientSecure client;

    // Test setup
    receivedRefresh = false;
    receivedControl = false;
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "");


    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, NO_LOGS);

    wappsto.connect();

    Network *myNetwork = wappsto.createNetwork("Basic Example", "");
    assertNotEqual(myNetwork, NULL);
    DeviceDescription_t myDeviceDescription = {
        .name = "My Device",
        .product = "ESP32 example",
        .manufacturer = "",
        .description = "ESP32 Example description",
        .version = "1.0",
        .serial = "00001",
        .protocol = "Json-RPC",
        .communication = "WiFi",
    };
    Device *myDevice = myNetwork->createDevice(&myDeviceDescription);
    assertNotEqual(myDevice, NULL);

    ValueString_t stringParameters = {  .name = "String",
                                        .type = "text",
                                        .permission = READ_WRITE,
                                        .max = 1,
                                        .encoding = "text"};

    Value *myStringValue = myDevice->createValueString(&stringParameters);
    myStringValue->onControl(&controlStringCallback);
    myStringValue->onRefresh(&refreshCallback);


    // Test refresh
    client.testRefresh("42b7bb41-bf32-4648-1102-aea6fca55643", "/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641/value/42b7bb41-bf32-4648-1102-aea6fca55642/state/");
    wappsto.dataAvailable();
    assertEqual(receivedRefresh, true);

    // Test control
    client.testControl("42b7bb41-bf32-4648-1102-aea6fca55644", "/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641/value/42b7bb41-bf32-4648-1102-aea6fca55642/state/", "Correct test string");
    wappsto.dataAvailable();
    assertEqual(receivedControl, true);

    client.testControl("42b7bb41-bf32-4648-1102-aea6fca55644", "/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641/value/42b7bb41-bf32-4648-1102-aea6fca55642/state/", "Incorrect");
    wappsto.dataAvailable();
    assertEqual(receivedControl, false);


    // Test report
    myStringValue->report("123");
    assertEqual("123", myStringValue->getReportData());

    // Test control
    myStringValue->control("123");
    assertEqual("123", myStringValue->getControlData());

    //client.testPrintAllReceivedData();
    // {"jsonrpc":"2.0","id":57006,"method":"POST","params":{"url":"/network","meta":{"fast":true},"data":{"name":"Basic Example","description":"","meta":{"id":"4906c6be-cc7f-4c4d-8806-60a38c5fcef5","type":"network","version":"2.0"}}}}{"jsonrpc":"2.0","id":57007,"method":"GET","params":{"url":"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Device"}}{"jsonrpc":"2.0","id":57008,"method":"PUT","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641","meta":{"fast":true},"data":{"name":"My Device","manufacturer":"","product":"ESP32 example","version":"1.0","serial":"00001","description":"ESP32 Example description","protocol":"Json-RPC","communication":"WiFi","meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55641","type":"device","version":"2.0"}}}}{"jsonrpc":"2.0","id":57009,"method":"GET","params":{"url":"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==String"}}{"jsonrpc":"2.0","id":57010,"method":"PUT","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642","meta":{"fast":true},"data":{"name":"String","permission":"rw","type":"text","period":"0","delta":"0","string":{"max":1,"encoding":"text"},"meta":{"id":"42b7bb41-bf32-4648-1102-aea6fca55642","type":"value","version":"2.0"}}}}{"jsonrpc":"2.0","id":57011,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report"}}{"jsonrpc":"2.0","id":57012,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643"}}{"jsonrpc":"2.0","id":57013,"method":"GET","params":{"url":"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control"}}{"jsonrpc":"2.0","id":57014,"method":"GET","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644"}}{"jsonrpc":"2.0","id":"TestRefresh","result":true}{"jsonrpc":"2.0","id":"TestControl","result":true}{"jsonrpc":"2.0","id":"TestControl","result":true}{"jsonrpc":"2.0","id":57015,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55643","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"123","type":"Report"}}}{"jsonrpc":"2.0","id":57016,"method":"PUT","params":{"url":"/state/42b7bb41-bf32-4648-1102-aea6fca55644","meta":{"fast":true},"data":{"timestamp":"2022-02-16T09:30:18Z","data":"123","type":"Control"}}}
    const char* test = "{\"jsonrpc\":\"2.0\",\"id\":57006,\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"Basic Example\",\"description\":\"\",\"meta\":{\"id\":\"4906c6be-cc7f-4c4d-8806-60a38c5fcef5\",\"type\":\"network\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57007,\"method\":\"GET\",\"params\":{\"url\":\"/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device?this_name==My Device\"}}{\"jsonrpc\":\"2.0\",\"id\":57008,\"method\":\"PUT\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"My Device\",\"manufacturer\":\"\",\"product\":\"ESP32 example\",\"version\":\"1.0\",\"serial\":\"00001\",\"description\":\"ESP32 Example description\",\"protocol\":\"Json-RPC\",\"communication\":\"WiFi\",\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55641\",\"type\":\"device\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57009,\"method\":\"GET\",\"params\":{\"url\":\"/device/42b7bb41-bf32-4648-1102-aea6fca55641/value?this_name==String\"}}{\"jsonrpc\":\"2.0\",\"id\":57010,\"method\":\"PUT\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642\",\"meta\":{\"fast\":true},\"data\":{\"name\":\"String\",\"permission\":\"rw\",\"type\":\"text\",\"period\":\"0\",\"delta\":\"0\",\"string\":{\"max\":1,\"encoding\":\"text\"},\"meta\":{\"id\":\"42b7bb41-bf32-4648-1102-aea6fca55642\",\"type\":\"value\",\"version\":\"2.0\"}}}}{\"jsonrpc\":\"2.0\",\"id\":57011,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Report\"}}{\"jsonrpc\":\"2.0\",\"id\":57012,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\"}}{\"jsonrpc\":\"2.0\",\"id\":57013,\"method\":\"GET\",\"params\":{\"url\":\"/value/42b7bb41-bf32-4648-1102-aea6fca55642/state?this_type==Control\"}}{\"jsonrpc\":\"2.0\",\"id\":57014,\"method\":\"GET\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\"}}{\"jsonrpc\":\"2.0\",\"id\":\"TestRefresh\",\"result\":true}{\"jsonrpc\":\"2.0\",\"id\":\"TestControl\",\"result\":true}{\"jsonrpc\":\"2.0\",\"id\":\"TestControl\",\"result\":true}{\"jsonrpc\":\"2.0\",\"id\":57015,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55643\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"123\",\"type\":\"Report\"}}}{\"jsonrpc\":\"2.0\",\"id\":57016,\"method\":\"PUT\",\"params\":{\"url\":\"/state/42b7bb41-bf32-4648-1102-aea6fca55644\",\"meta\":{\"fast\":true},\"data\":{\"timestamp\":\"2022-02-16T09:30:18Z\",\"data\":\"123\",\"type\":\"Control\"}}}";
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