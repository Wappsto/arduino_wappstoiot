#include <AUnit.h>
#include <Wappsto.h>

static bool receivedControl = false;
static bool receivedRefresh = false;

void controlLedCallback(Value *value, double data, String timestamp)
{
    receivedControl = true;
}

void refreshCallback(Value *value)
{
    receivedRefresh = true;
}

test(callbackTest) {
    WiFiClientSecure client;

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

    ValueNumber_t myLedParameters = {   .name = "Led",
                                        .type = "light",
                                        .permission = READ_WRITE,
                                        .min = 0,
                                        .max = 1,
                                        .step = 1,
                                        .unit = "",
                                        .si_conversion = ""};


    Value *myLedValue = myDevice->createValueNumber(&myLedParameters);
    myLedValue->onControl(&controlLedCallback);
    myLedValue->onRefresh(&refreshCallback);

    client.testRefresh("42b7bb41-bf32-4648-1102-aea6fca55643", "/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641/value/42b7bb41-bf32-4648-1102-aea6fca55642/state/");
    wappsto.dataAvailable();
    assertEqual(receivedRefresh, true);

    client.testControl("42b7bb41-bf32-4648-1102-aea6fca55644", "/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641/value/42b7bb41-bf32-4648-1102-aea6fca55642/state/", "1");
    wappsto.dataAvailable();
    assertEqual(receivedControl, true);

    //myLedValue->report(1);
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