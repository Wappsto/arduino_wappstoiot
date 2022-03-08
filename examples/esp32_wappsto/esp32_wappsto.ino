/** Getting started with ESP32

https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
- Add to preferences, additional boards manager urls: https://dl.espressif.com/dl/package_esp32_index.json
- Board manager, search for ESP32 and install

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
Value *myLedValue;
int myLed = 0;
#define LED_PIN 2

DeviceDescription_t myDeviceDescription = {
    .name = "My Device",
    .product = "ESP32 example",
    .manufacturer = "",
    .description = "ESP32 Example description",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};

ValueNumber_t myLedParameters = {   .name = "Led",
                                            .type = "light",
                                            .permission = READ_WRITE,
                                            .min = 0,
                                            .max = 1,
                                            .step = 1,
                                            .unit = "",
                                            .si_conversion = ""};

void controlLedCallback(Value *value, double data, String timestamp)
{
    myLed = (int)data;
    digitalWrite(LED_PIN, myLed);
    value->report(myLed);
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

    pinMode(LED_PIN, OUTPUT);

    initializeWifi();

    initializeNtp();

    wappsto.config(network_uuid, ca, client_crt, client_key, 5, NO_LOGS);
    if(wappsto.connect()) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
    }

    // Create network
    myNetwork = wappsto.createNetwork("ESP32 Basic Example");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create LED value
    myLedValue = myDevice->createValueNumber(&myLedParameters);
    myLedValue->onControl(&controlLedCallback);

    // Get the last control request, and set the led to this value
    myLed = myLedValue->getControlData().toInt();
    digitalWrite(LED_PIN, myLed);
    myLedValue->report(myLed);
}

void loop()
{
    delay(200);
    wappsto.dataAvailable();
}
