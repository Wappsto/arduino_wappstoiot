#include <ArduinoJson.h>
#include "rpcWiFi.h" // Seeed arduino rpc unified, seeed arduino rpcwifi, seeed arduino rpcble
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include "RTC_SAMD51.h" // arduino lib: Seeed_Arduino_RTC
#include "DateTime.h"

#include "WappstoIoT.h"

WiFiMulti WiFiMulti;
WiFiClientSecure client;
Network wappsto(&client);

unsigned int localPort = 2390;      // local port to listen for UDP packets
char timeServer[] = "time.nist.gov"; // extenral NTP server e.g. time.nist.gov
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// declare a time object
DateTime now;
WiFiUDP udp;
unsigned long devicetime;
RTC_SAMD51 rtc;


const char* ssid = "";
const char* password = "";

const char* network_id = "ae9f973e-22b9-4c2a-9de5-b9eb5db64c3d";

const char* ca = "-----BEGIN CERTIFICATE-----\n    \n-----END CERTIFICATE-----\n";
const char* client_crt = "-----BEGIN CERTIFICATE-----\n  \n-----END CERTIFICATE-----\n";
const char* client_key = "-----BEGIN RSA PRIVATE KEY-----\n  \n-----END RSA PRIVATE KEY-----\n";

Device *myDevice;
Value *myNumberValue;
Value *myStringValue;
ValueNumber_t myNumberValueParameters;
ValueString_t myStringValueParameters;

int counter = 0;

#define UUID_COUNT 7
uint32_t deviceIndex[] = {
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

static char strfTimeBuffer[21];

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
    counter++;
    val->report(String(counter));
}

void refreshNumberCallback(void *object, String data)
{
    Value *val = (Value*)object;
    Serial.print("Refresh callback: ");
    counter++;
    val->report(String(counter));
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

unsigned long getNTPtime()
{
    // module returns a unsigned long time valus as secs since Jan 1, 1970
    // unix time or 0 if a problem encounted

    //only send data when connected
    if (WiFi.status() == WL_CONNECTED) {
        //initializes the UDP state
        //This initializes the transfer buffer
        udp.begin(WiFi.localIP(), localPort);

        sendNTPpacket(timeServer); // send an NTP packet to a time server
        // wait to see if a reply is available
        delay(1000);

        if (udp.parsePacket()) {
            // We've received a packet, read the data from it
            udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

            //the timestamp starts at byte 40 of the received packet and is four bytes,
            // or two words, long. First, extract the two words:

            unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
            unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
            // combine the four bytes (two words) into a long integer
            // this is NTP time (seconds since Jan 1 1900):
            unsigned long secsSince1900 = highWord << 16 | lowWord;
            // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
            const unsigned long seventyYears = 2208988800UL;
            // subtract seventy years:
            unsigned long epoch = secsSince1900 - seventyYears;

            return epoch;
        }
        else {
            // were not able to parse the udp packet successfully
            // clear down the udp connection
            udp.stop();
            return 0; // zero indicates a failure
        }
        // not calling ntp time frequently, stop releases resources
        udp.stop();
    }
    else {
        // network not connected
        return 0;
    }
}


 // send an NTP request to the time server at the given address
unsigned long sendNTPpacket(const char* address)
{
    // set all bytes in the buffer to 0
    for (int i = 0; i < NTP_PACKET_SIZE; ++i) {
        packetBuffer[i] = 0;
    }
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    udp.beginPacket(address, 123); //NTP requests are to port 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
}

void initializeNtp(void)
{
    // get the time via NTP (udp) call to time server
    // getNTPtime returns epoch UTC time adjusted for timezone but not daylight savings
    // time
    devicetime = getNTPtime();

    // check if rtc present
    if (devicetime == 0) {
        Serial.println("Failed to get time from network time server.");
    }

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1) delay(10); // stop operating
    }

    // adjust time using ntp time
    rtc.adjust(DateTime(devicetime));

    // get and print the adjusted rtc time
    now = rtc.now();
    Serial.print("UTC (boot) time is: ");
    Serial.println(now.timestamp(DateTime::TIMESTAMP_FULL));
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect.
    }
    randomSeed(analogRead(0));
    initializeWifi();

    initializeNtp();

    if(wappsto.connect("test", network_id, ca, client_crt, client_key)) {
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

    myDevice = wappsto.createDevice("My Device", "", "", "", "", "");

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

    //myNumberValue->report(String(counter));
    //counter++;
}
