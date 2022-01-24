#include <AUnit.h>
#include <Wappsto.h>

test(InvalidWifiClient) {
    Wappsto wappsto(0);
}

test(InvalidParametersShort) {
    WiFiClientSecure client;
    Wappsto wappsto(&client);

    int r = wappsto.config(0,0,0,0);
    assertEqual(r, -1);

    r = wappsto.config("",0,0,0);
    assertEqual(r, -1);
}

test(InvalidParametersLong) {
    WiFiClientSecure client;
    Wappsto wappsto(&client);

    int r = wappsto.config(0,0,0,0,0,VERBOSE);
    assertEqual(r, -1);
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