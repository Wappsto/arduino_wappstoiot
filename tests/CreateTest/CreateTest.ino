#include <AUnit.h>
#include <Wappsto.h>


test(createNetwork) {
    WiFiClientSecure client;
    Wappsto wappsto(&client);
    wappsto.config("4906c6be-cc7f-4c4d-8806-60a38c5fcef5", "", "", "", 0, VERBOSE);

    wappsto.connect();

    Network *myNetwork = wappsto.createNetwork("Basic Example", "hest");
    assertNotEqual(myNetwork, NULL);
    assertEqual(myNetwork->name, "Basic Example");
    int len = 36; //strlen(myNetwork->uuid);
    assertEqual(len, 36);
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