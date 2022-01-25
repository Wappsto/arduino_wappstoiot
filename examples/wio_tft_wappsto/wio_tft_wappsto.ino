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

**/

#include "rpcWiFi.h"
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include"TFT_eSPI.h"

#include "Wappsto.h"
#include "wappsto_config.h"

#include "WappstoValueTypes.h"


TFT_eSPI tft;
WiFiMulti WiFiMulti;
WiFiClientSecure client;
Wappsto wappsto(&client);

const char* ssid = "";
const char* password = "";

Network *myNetwork;
Device *myDevice;
Value *displayStrValue;
Value *backgroundColorValue;

int backgroundColor = 0x00FF00;
String textString = "";

DeviceDescription_t myDeviceDescription = {
    .name = "My Demo Device",
    .product = "WIO Terminal example",
    .manufacturer = "",
    .description = "Examle device",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};
ValueString_t displayStrValueParameters = { .name = "Display Sting",
                                            .type = "string",
                                            .permission = READ_WRITE,
                                            .max = 200,
                                            .encoding = ""};

const uint16_t red_mask = 0xF800;
const uint16_t green_mask = 0x7E0;
const uint16_t blue_mask = 0x1F;

uint16_t convert24bitColorTo16bit(uint32_t color)
{
    uint16_t color565;
    uint8_t red_value = (backgroundColor & 0xFF0000) >> 16;
    uint8_t green_value = (backgroundColor & 0x00FF00) >> 8;
    uint8_t blue_value = (backgroundColor & 0xFF);
    red_value = (uint8_t)(red_value * 31 / 0xFF);
    green_value = (uint8_t)(green_value * 63 / 0xFF);
    blue_value = (uint8_t)(blue_value * 31 / 0xFF);
    color565 = (red_value << 11) | (green_value << 5) | blue_value;
    return color565;
}

void controlStringCallback(Value *value, String data, String timestamp)
{
    tft.fillScreen(convert24bitColorTo16bit(backgroundColor));
    textString = data;
    tft.drawString(textString, 10, 10);
    value->report(data);
}

void refreshStringCallback(Value *value)
{
    tft.fillScreen(convert24bitColorTo16bit(backgroundColor));
    textString = "Refreshed";
    tft.drawString(textString, 10, 10);
    value->report(textString);
}

void backgroundColorRefresh(Value *value)
{
    value->report(backgroundColor);
}

void deleteNetworkCallback(Network *network)
{
    Serial.println("Network deleted, need to restart device, and claim network again");
}

void backgroundColorControl(Value *value, String data, String timestamp)
{
    backgroundColor = data.toInt();
    tft.fillScreen(convert24bitColorTo16bit(backgroundColor));
    tft.drawString(textString, 10, 10);
    value->report(backgroundColor);
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

    // Screen update
    tft.begin();
    tft.setRotation(3);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.fillScreen(TFT_RED);

    randomSeed(analogRead(0));

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

    wappsto.config(network_uuid, ca, client_crt, client_key);
    if(wappsto.connect()) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
        // Screen update
        tft.fillScreen(TFT_RED);
        tft.drawString("Could not connect to Wappsto", 10, 10);
    }

    // Create network
    myNetwork = wappsto.createNetwork("Wio TFT Wappsto");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create r/w value for display string
    displayStrValue = myDevice->createValueString(&displayStrValueParameters);
    displayStrValue->onControl(&controlStringCallback);
    displayStrValue->onRefresh(&refreshStringCallback);
    textString = "Connected";
    displayStrValue->report(textString);

    backgroundColorValue = myDevice->createValueBlob(&defaultColorParameter);
    backgroundColorValue->onRefresh(&backgroundColorRefresh);
    backgroundColorValue->onControl(&backgroundColorControl);
    backgroundColorValue->report(backgroundColor);
    backgroundColorValue->control(backgroundColor);

    // Screen update
    tft.fillScreen(convert24bitColorTo16bit(backgroundColor));
    tft.drawString(textString, 10, 10);
}

void loop()
{
    delay(200);
    wappsto.dataAvailable(); // Required to receive data from Wappsto
}
