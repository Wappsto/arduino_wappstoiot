/** Generic echo example without a target defined

To run on a target see the examples for ESP32 and WIO terminal.

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
Value *myEchoStringValue;

DeviceDescription_t myDeviceDescription = {
    .name = "Echo Device",
    .product = "",
    .manufacturer = "",
    .description = "Take control string and writes it report and send back",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};
ValueString_t echoStringValue = { .name = "Echo String",
                                    .type = "string",
                                    .permission = READ_WRITE,
                                    .max = 200,
                                    .encoding = ""};

void controlEchoCallback(Value *value, String data, String timestamp)
{
    value->report(data);
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
    myNetwork = wappsto.createNetwork("Echo String Example");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create echo string value
    myEchoStringValue = myDevice->createStringValue(&echoStringValue);
    myEchoStringValue->onControl(&controlEchoCallback);
    myEchoStringValue->report("");
}


void loop()
{
    delay(500);
    wappsto.dataAvailable();
}