#include <AUnit.h>
#include <Wappsto.h>

static char strfTimeBuffer[21] = "";
const char* getUtcTime(void)
{
    return strfTimeBuffer;
}

test(InvalidWifiClient) {
    Wappsto wappsto(0);
}

test(InvalidParametersShort) {
    WiFiClientSecure client;
    Wappsto wappsto(&client);
    wappsto.config(0,0,0,0);
}

test(InvalidParametersLong) {
    WiFiClientSecure client;
    Wappsto wappsto(&client);
    wappsto.config(0,0,0,0,0,VERBOSE);
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