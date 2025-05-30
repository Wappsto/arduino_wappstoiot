/** Generic example showing how to send a temperature every 15 minutes

To run on a target see the examples for ESP32 and WIO terminal.

**/

#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include "Wappsto.h"
#include "wappsto_config.h"

#include "WappstoValueTypes.h"

WiFiMulti WiFiMulti;
WiFiClientSecure client;
Wappsto wappsto(&client);

const char* ssid = "";
const char* password = "";

WNetwork *myNetwork;
Device *myDevice;
Value *myTemperatureValue;
int myLed = 0;
#define LED_PIN 2

DeviceDescription_t myDeviceDescription = {
    .name = "Temperature Device",
    .product = "",
    .manufacturer = "",
    .description = "Example sending temperaure at a interval",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};

double myTemperatureReading = 21.3;

unsigned long startMillis = 0;
unsigned long currentMillis = 0;
#define UPDATE_INTERVAL_MS 15*60*1000 // 15 minutes in milliseconds

void refreshTemperatureCallback(Value *value)
{
    value->report(myTemperatureReading);
}

void updateTemperature(void)
{
    // Here you could read a temperature sensor, and update the value
    myTemperatureReading = myTemperatureReading + 1;
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

    initializeWifi();

    initializeNtp();

    wappsto.config(network_uuid, ca, client_crt, client_key, 5, NO_LOGS);
    if(wappsto.connect()) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
    }

    // Create network
    myNetwork = wappsto.createNetwork("Temperature Example");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create echo string value
    myTemperatureValue = myDevice->createNumberValue(&defaultTemperatureParameter);
    myTemperatureValue->onRefresh(&refreshTemperatureCallback);
    myTemperatureValue->report(myTemperatureReading);
}

void loop()
{
    delay(500);
    wappsto.dataAvailable();
    currentMillis = millis();
    if(currentMillis - startMillis >= UPDATE_INTERVAL_MS) {
        startMillis = currentMillis;
        updateTemperature();
        myTemperatureValue->report(myTemperatureReading);
    }
}
