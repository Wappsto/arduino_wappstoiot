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

#include"TFT_eSPI.h"

#include "Wappsto.h"
#include "wappsto_setup.h"

#define BUZZER_PIN WIO_BUZZER

TFT_eSPI tft;
WiFiMulti WiFiMulti;
WiFiClientSecure client;
Wappsto wappsto(&client);

const char* ssid = "";
const char* password = "";

Network *myNetwork;
Device *myDevice;
Value *myNumberValue;
Value *displayStrValue;
Value *buttonValue;

DeviceDescription_t myDeviceDescription = {
    .product = "WIO Terminal example",
    .manufacturer = "",
    .description = "Examle device",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};
ValueNumber_t myNumberValueParameters = {.min = 100, .max = 4000, .step = 1, .unit = "", .si_conversion = ""};
ValueString_t displayStrValueParameters = {.max = 200, .encoding = ""};
ValueString_t buttonStringParameters = {.max = 6, .encoding = ""};


void controlNumberCallback(Value *value, double data, String timestamp)
{
    playTone((int)data, 1000);
    value->report(data);
}

void refreshNumberCallback(Value *value)
{
    Serial.print("Refresh callback: ");
    value->report(value->getReportData());
}

void controlStringCallback(Value *value, String data, String timestamp)
{
    tft.fillScreen(TFT_GREEN);
    tft.drawString(data, 10, 10);
    value->report(data);
}

void refreshStringCallback(Value *value)
{
    tft.fillScreen(TFT_GREEN);
    tft.drawString("Value was refreshed", 10, 10);
    value->report(String("Refresh"));
}

void buttonRefresh(Value *value)
{
    value->report(String(""));
}

void deleteNetworkCallback(Network *network)
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

void playTone(int tone, int duration)
{
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(tone);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(tone);
    }
}

void initialiseWioOutput(void)
{
    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);
    pinMode(WIO_5S_LEFT, INPUT_PULLUP);
    pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
}

void setup()
{
    Serial.begin(115200);

    // Screen update
    tft.begin();
    tft.setRotation(3);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.fillScreen(TFT_RED);

    randomSeed(analogRead(0));
    initialiseWioOutput();

    // Screen update
    tft.drawString("Connecting to WiFi", 10, 10);

    initializeWifi();

    // Screen update
    tft.drawString(WiFi.localIP().toString(), 10, 40);
    tft.drawString("Getting NTP", 10, 70);

    initializeNtp();

    // Screen update
    tft.fillScreen(TFT_YELLOW);
    tft.drawString("Connecting to Wappsto", 10, 10);

    wappsto.config(network_id, ca, client_crt, client_key);
    if(wappsto.connect()) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
        // Screen update
        tft.fillScreen(TFT_RED);
        tft.drawString("Could not connect to Wappsto", 10, 10);
    }

    // Create network
    myNetwork = wappsto.createNetwork("Demo");

    // Create device
    myDevice = myNetwork->createDevice("My Demo Device", &myDeviceDescription);

    // Create r/w number to play tone
    myNumberValue = myDevice->createValueNumber("Beep number", "audio number", READ_WRITE, &myNumberValueParameters);
    myNumberValue->onControl(&controlNumberCallback);
    myNumberValue->onRefresh(&refreshNumberCallback);

    // Create r/w value for display string
    displayStrValue = myDevice->createValueString("Display Sting", "string", READ_WRITE, &displayStrValueParameters);
    displayStrValue->onControl(&controlStringCallback);
    displayStrValue->onRefresh(&refreshStringCallback);

    // Create r value for button output
    buttonValue = myDevice->createValueString("Button direction", "string", READ, &buttonStringParameters);
    buttonValue->onRefresh(&buttonRefresh);

    // Screen update
    tft.fillScreen(TFT_GREEN);
    tft.drawString("Connected", 10, 10);
}

void loop()
{
    wappsto.dataAvailable(); // Required to receive data from Wappsto

    if (digitalRead(WIO_5S_UP) == LOW) {
        buttonValue->report("Up");
    } else if (digitalRead(WIO_5S_DOWN) == LOW) {
        buttonValue->report("Down");
    } else if (digitalRead(WIO_5S_LEFT) == LOW) {
        buttonValue->report("Left");
    } else if (digitalRead(WIO_5S_RIGHT) == LOW) {
        buttonValue->report("Right");
    } else if (digitalRead(WIO_5S_PRESS) == LOW) {
        buttonValue->report("Press");
    }
    delay(200);
}
