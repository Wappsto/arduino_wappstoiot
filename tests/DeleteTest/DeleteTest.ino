#include <AUnit.h>
#include <Wappsto.h>

bool receivedDeleteDevice = false;
bool receivedDeleteNetwork = false;

void deleteDeviceCallback(Device *device)
{
    (void)device;
    receivedDeleteDevice = true;
}

void deleteNetworkCallback(Network *network)
{
    (void)network;
    receivedDeleteNetwork = true;
}

test(deleteDeviceThenNetwork) {
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

    myNetwork->onDelete(&deleteNetworkCallback);
    myDevice->onDelete(&deleteDeviceCallback);

    client.testDelete("/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5/device/42b7bb41-bf32-4648-1102-aea6fca55641");
    wappsto.dataAvailable();
    assertEqual(receivedDeleteDevice, true);

    client.testDelete("/network/4906c6be-cc7f-4c4d-8806-60a38c5fcef5");
    wappsto.dataAvailable();
    assertEqual(receivedDeleteNetwork, true);
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
