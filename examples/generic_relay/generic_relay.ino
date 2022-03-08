/** Generic relay example without a target defined

To run on a target see the examples for ESP32 and WIO terminal.

**/

#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include "Wappsto.h"
#include "wappsto_config.h"

WiFiMulti WiFiMulti;
WiFiClientSecure client;
Wappsto wappsto(&client);

const char* ssid = "";
const char* password = "";

Network *myNetwork;
Device *myDevice;
Value *myRelayValue;
int myRelay = 0;
#define RELAY_PIN 2

DeviceDescription_t myDeviceDescription = {
    .name = "My Relay Device",
    .product = "",
    .manufacturer = "",
    .description = "Example controlling a relay",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};

ValueNumber_t myRelayParameters = {   .name = "Relay",
                                            .type = "relay",
                                            .permission = READ_WRITE,
                                            .min = 0,
                                            .max = 1,
                                            .step = 1,
                                            .unit = "",
                                            .si_conversion = ""};

void controlRelayCallback(Value *value, double data, String timestamp)
{
    myRelay = (int)data;
    digitalWrite(RELAY_PIN, myRelay);
    value->report(myRelay);
}

void initializeWifi(void)
{
    Serial.println("Initializing WiFi");
    WiFiMulti.addAP(ssid, password);
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);
    randomSeed(analogRead(0));

    pinMode(RELAY_PIN, OUTPUT);

    initializeWifi();

    initializeNtp();

    wappsto.config(network_uuid, ca, client_crt, client_key, 5, NO_LOGS);
    if(wappsto.connect()) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
    }

    // Create network
    myNetwork = wappsto.createNetwork("Relay Example");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create LED value
    myRelayValue = myDevice->createValueNumber(&myRelayParameters);
    myRelayValue->onControl(&controlRelayCallback);

    // Get the last control request, and set the led to this value
    myRelay = myRelayValue->getControlData().toInt();
    digitalWrite(RELAY_PIN, myRelay);
    myRelayValue->report(myRelay);
}

void loop()
{
    delay(200);
    wappsto.dataAvailable();
}
