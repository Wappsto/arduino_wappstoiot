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
}

test(createNetworkFailNotConnected) {
    WiFiClientSecure client;

    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, NO_LOGS);

    Network *myNetwork = wappsto.createNetwork("Basic Example", "");
    assertEqual(myNetwork, NULL);
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
}

test(createValueString) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "Test data");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "Test data");

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
}

test(createValueBlob) {
    WiFiClientSecure client;

    // Test setup
    client.addDeviceUuid("42b7bb41-bf32-4648-1102-aea6fca55641");
    client.addValueUuid("42b7bb41-bf32-4648-1102-aea6fca55642");
    client.addReportUuid("42b7bb41-bf32-4648-1102-aea6fca55643", "04FFEA");
    client.addControlUuid("42b7bb41-bf32-4648-1102-aea6fca55644", "04FFEA");

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
