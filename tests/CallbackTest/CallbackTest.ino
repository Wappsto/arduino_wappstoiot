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


    ValueString_t stringParameters = {  .name = "String",
                                        .type = "text",
                                        .permission = READ_WRITE,
                                        .max = 1};

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