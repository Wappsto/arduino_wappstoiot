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

#include <ArduinoJson.h>
#include "rpcWiFi.h"
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include "RTC_SAMD51.h" // arduino lib: Seeed_Arduino_RTC
#include "DateTime.h"

#include "Wappsto.h"


WiFiMulti WiFiMulti;
WiFiClientSecure client;
Wappsto wappsto(&client);

unsigned int localPort = 2390;      // local port to listen for UDP packets
char timeServer[] = "time.nist.gov"; // extenral NTP server e.g. time.nist.gov
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// declare a time object
DateTime now;
WiFiUDP udp;
unsigned long devicetime;
RTC_SAMD51 rtc;

static char strfTimeBuffer[21];

const char* ssid = "";
const char* password = "";

const char* network_id = "ae9f973e-22b9-4c2a-9de5-b9eb5db64c3d";

const char* ca = "-----BEGIN CERTIFICATE-----\n    \n-----END CERTIFICATE-----\n";
const char* client_crt = "-----BEGIN CERTIFICATE-----\n  \n-----END CERTIFICATE-----\n";
const char* client_key = "-----BEGIN RSA PRIVATE KEY-----\n  \n-----END RSA PRIVATE KEY-----\n";

Network *myNetwork;
Device *myDevice;
Value *myNumberValue;
Value *myStringValue;
ValueNumber_t myNumberValueParameters;
ValueString_t myStringValueParameters;

int myNumberCounter = 0;

#define UUID_COUNT 7
uint32_t deviceIndex[UUID_COUNT] = {
    0x0000FFFF,
    0x000000FF,
    0x00000001,
    0x00000002,
    0x000001FF,
    0x00000101,
    0x00000102,
};

const char uuidList[UUID_COUNT][UUID_LENGTH] {
    "07b3f6ca-b24a-4ba7-bcf3-a39030482eaf",
    "3f985158-ec31-4025-b6ee-8081366ceba1",
    "1a3fd4a1-7172-44fa-2de2-af68688e126b",
    "5851c976-b200-4ae0-3d02-b93f0f974c7a",
    "fbb2a8ea-7c48-440c-b453-4fa6a316f120",
    "afefd0b7-d207-4cff-9caa-e99a029c887c",
    "6947f6d8-5ccc-4c5e-8e50-66469a37fe04",
};

bool writeUuid(uint32_t storeId, const char* uuid)
{
    Serial.print("Need to save this UUID: 0x");
    Serial.print(storeId, HEX);
    Serial.print(" - uuid: ");
    Serial.println(uuid);
    return true;
}

const char* readUuid(uint32_t storeId)
{
    for(int i=0; i<UUID_COUNT; i++) {
        if(storeId == deviceIndex[i]) {
            Serial.print("Found UUID: ");
            Serial.println(storeId, HEX);
            return uuidList[i];
        }
    }
    Serial.print("Did not find UUID: ");
    Serial.println(storeId, HEX);
    return NULL;
}


const char* getUtcTime(void)
{
    now = rtc.now();
    time_t t = now.unixtime();
    struct tm *timeinfo = localtime(&t);
    strftime(strfTimeBuffer, sizeof(strfTimeBuffer), "%FT%TZ", timeinfo);
    Serial.println(strfTimeBuffer);
    return strfTimeBuffer;
}

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
    while (!Serial) {
        ; // wait for serial port to connect.
    }
    randomSeed(analogRead(0));
    initializeWifi();

    initializeNtp();

    if(wappsto.connect(network_id, ca, client_crt, client_key)) {
        Serial.println("Connected to Wappsto");
    } else {
        Serial.println("Could not connect");
    }
    myNumberValueParameters.min = 0;
    myNumberValueParameters.max = 100;
    myNumberValueParameters.step = 1;
    myNumberValueParameters.unit = "";
    myNumberValueParameters.si_conversion = "";

    myStringValueParameters.max = 200;
    myStringValueParameters.encoding = "";

    myNetwork = wappsto.createNetwork("test");

    myDevice = myNetwork->createDevice("My Device", "", "", "", "", "");

    myNumberValue = myDevice->createValueNumber("My number", "test number", READ_WRITE, &myNumberValueParameters);
    myNumberValue->onControl(&controlNumberCallback);
    myNumberValue->onRefresh(&refreshNumberCallback);

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
