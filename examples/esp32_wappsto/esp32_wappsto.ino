/** Getting started with ESP32

https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
- Add to preferences, additional boards manager urls: https://dl.espressif.com/dl/package_esp32_index.json
- Board manager, search for ESP32 and install

WappstoIoT Libraries requirements
ArduinoJson
WiFiClientSecure
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
Value *myTemperatureValue;

DeviceDescription_t myDeviceDescription = {
    .name = "My Device",
    .product = "ESP32 example",
    .manufacturer = "",
    .description = "Example description",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};
ValueNumber_t myTemperatureParameters = {   .name = "Temperature",
                                            .type = "temperature",
                                            .permission = READ,
                                            .min = -20,
                                            .max = 100,
                                            .step = 0.1,
                                            .unit = "°C",
                                            .si_conversion = ""};

double myTemperatureReading = 21.3;

void refreshTemperatureCallback(Value *value)
{
    value->report(myTemperatureReading);
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

void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(0));
    initializeWifi();

    initializeNtp();

    wappsto.config(network_uuid, ca, client_crt, client_key);
    if(wappsto.connect()) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
    }

    // Create network
    myNetwork = wappsto.createNetwork("Basic Example");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create temperature value
    myTemperatureValue = myDevice->createValueNumber(&myTemperatureParameters);
    myTemperatureValue->onRefresh(&refreshTemperatureCallback);
}

unsigned long startMillis = 0;
unsigned long currentMillis = 0;
#define UPDATE_INTERVAL_MILLIS 2*60*1000

void loop() {
    delay(500);
    wappsto.dataAvailable();

    currentMillis = millis();
    if(currentMillis - startMillis >= UPDATE_INTERVAL_MILLIS) {
        startMillis = currentMillis;
        myTemperatureValue->report(myTemperatureReading);
    }
}
