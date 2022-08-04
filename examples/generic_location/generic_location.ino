/** Generic example showing how to send a location

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

Network *myNetwork;
Device *myDevice;
Value *myLatitudeValue;
Value *myLongitudeValue;

DeviceDescription_t myDeviceDescription = {
    .name = "Location Device",
    .product = "",
    .manufacturer = "",
    .description = "Example sending location shown on map in the Wappsto dashboard",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};

double myLatitude = 57.019660;
double myLongitude = 9.883916;

void updateLocation(void)
{
    myLatitudeValue->report(myLatitude);
    myLongitudeValue->report(myLongitude);
}

void refreshLocationCallback(Value *value)
{
    updateLocation();
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
    myNetwork = wappsto.createNetwork("Location Example");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create a default temperature value
    myLatitudeValue = myDevice->createNumberValue(&defaultLatitudeParameter);
    myLongitudeValue = myDevice->createNumberValue(&defaultLongitudeParameter);
    myLatitudeValue->onRefresh(&refreshLocationCallback);
    myLongitudeValue->onRefresh(&refreshLocationCallback);
    updateLocation();
}

void loop()
{
    delay(500);
    wappsto.dataAvailable();
}
