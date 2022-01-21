#include <AUnit.h>
#include <Wappsto.h>

static char strfTimeBuffer[21] = "";
const char* getUtcTime(void)
{
    return strfTimeBuffer;
}

test(createNetwork) {
    WiFiClientSecure client;
    Wappsto wappsto(&client);
    Network *myNetwork = wappsto.createNetwork("Basic Example");
    assertEqual(myNetwork->name, "Basic Example");
    int len = strlen(myNetwork->uuid);
    assertEqual(len, 47);
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