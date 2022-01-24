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

#include "Wappsto.h"
#include "wappsto_config.h"

#define BUZZER_PIN WIO_BUZZER

WiFiMulti WiFiMulti;
WiFiClientSecure client;
Wappsto wappsto(&client);

const char* ssid = "";
const char* password = "";

Network *myNetwork;
Device *myDevice;
Value *myNumberValue;
Value *buttonValue;

DeviceDescription_t myDeviceDescription = {
    .name = "My Device",
    .product = "WIO Terminal example",
    .manufacturer = "",
    .description = "Examle device",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};
ValueNumber_t myNumberValueParameters = {   .name = "Beep number",
                                            .type = "audio number",
                                            .permission = READ_WRITE,
                                            .min = 100,
                                            .max = 4000,
                                            .step = 1,
                                            .unit = "",
                                            .si_conversion = ""};
ValueString_t buttonStringParameters = {    .name = "Button direction",
                                            .type = "string",
                                            .permission = READ,
                                            .max = 6,
                                            .encoding = ""};


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

void buttonRefresh(Value *value)
{
    value->report(String(""));
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
    initialiseWioOutput();

    initializeWifi();

    initializeNtp();

    wappsto.config(network_uuid, ca, client_crt, client_key);
    if(wappsto.connect()) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
    }

    // Create network
    myNetwork = wappsto.createNetwork("Wio Example");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create r/w number to play tone
    myNumberValue = myDevice->createValueNumber(&myNumberValueParameters);
    myNumberValue->onControl(&controlNumberCallback);
    myNumberValue->onRefresh(&refreshNumberCallback);

    // Create r value for button output
    buttonValue = myDevice->createValueString(&buttonStringParameters);
    buttonValue->onRefresh(&buttonRefresh);
}

void loop()
{
    wappsto.dataAvailable();

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
