/** Getting started with Wio terminal
Important step for new hardware - wifi firmware upgrade:
https://wiki.seeedstudio.com/Wio-Terminal-Network-Overview/


https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started/
- Add to preferences, additional boards manager urls: https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json
- Board manager, search for wio, install Seeed SAMD Boards

Libraries
Seeed_Arduino_RTC
Seeed_Arduino_rpcWiFi - search for "seeed rpcwifi"
Seeed_Arduino_rpcUnified - search for "seeed rpcunified"
Seeed_Arduino_mbedtls - search for "seeed mbedtls"
Seeed_Arduino_FS - search for "seeed fs"
Seeed_Arduino_SFUD - search for "seeed sfud"

WappstoIoT Libraries requirements
ArduinoJson
WiFiClientSecure
**/

#include "rpcWiFi.h"
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include "RTC_SAMD51.h" // arduino lib: Seeed_Arduino_RTC
#include "DateTime.h"

#include "Wappsto.h"


WiFiMulti WiFiMulti;
WiFiClientSecure client;
Wappsto wappsto(&client);

RTC_SAMD51 rtc;

const char* ssid = "";
const char* password = "";

Network *myNetwork;
Device *myDevice;
Value *myNumberValue;
Value *myStringValue;
ValueNumber_t myNumberValueParameters = {.min = 100, .max = 4000, .step = 1, .unit = "", .si_conversion = ""};
ValueString_t myStringValueParameters = {.max = 200, .encoding = ""};

int myNumberCounter = 0;

void controlNumberCallback(void *object, String data)
{
    Value *val = (Value*)object;
    Serial.print("Control callback: ");
    Serial.println(data);
    myNumberCounter++;
    val->report(String(myNumberCounter));
}

void refreshNumberCallback(void *object, String data)
{
    Value *val = (Value*)object;
    Serial.print("Refresh callback: ");
    myNumberCounter++;
    val->report(String(myNumberCounter));
}

void controlStringCallback(void *object, String data)
{
    Value *val = (Value*)object;
    Serial.print("Control callback: ");
    Serial.println(data);
    val->report(data);
}

void refreshStringCallback(void *object, String data)
{
    Value *val = (Value*)object;
    Serial.print("Refresh callback: ");
    val->report(String("Refresh"));
}

void deleteNetworkCallback(void *object, String data)
{
    Serial.println("Network deleted, need to restart device, and claim network again");
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

    if(wappsto.connect(network_id, ca, client_crt, client_key)) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
    }

    // Create network
    myNetwork = wappsto.createNetwork("test");

    // Create device
    myDevice = myNetwork->createDevice("My Device", "", "", "", "", "");

    // Create r/w value, number
    myNumberValue = myDevice->createValueNumber("My number", "test number", READ_WRITE, &myNumberValueParameters);
    myNumberValue->onControl(&controlNumberCallback);
    myNumberValue->onRefresh(&refreshNumberCallback);

    // Create r/w value, string
    myStringValue = myDevice->createValueString("My string", "test string", READ_WRITE, &myStringValueParameters);
    myStringValue->onControl(&controlStringCallback);
    myStringValue->onRefresh(&refreshStringCallback);
}


void loop() {
    delay(500);
    wappsto.dataAvailable();

    //myNumberValue->report(String(myNumberCounter));
    //myNumberCounter++;
}
