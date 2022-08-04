/** Generic mode example without a target defined

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
Value *myModeValue;
int myMode = 0;

DeviceDescription_t myDeviceDescription = {
    .name = "My Mode selector",
    .product = "",
    .manufacturer = "",
    .description = "Example showing mode and mapping",
    .version = "1.0",
    .serial = "00001",
    .protocol = "Json-RPC",
    .communication = "WiFi",
};

const KEY_VALUE_t exampleModes[] =  {   {"0", "Disabled"},
                                        {"1", "Mode 1"},
                                        {"2", "Mode 2"},
                                        {"3", "Mode 3"},
                                        {"4", "Mode 4"},
                                        {"5", "Mode 5"},
                                        {"6", "Mode 6"},
};

const NUMBER_MAPPING_t myMapping  = { .size = 7,
                                     .map = exampleModes,
};

ValueNumberFull_t myModeParameters = {  .name = "Mode Example",
                                        .type = "mode",
                                        .permission = READ_WRITE,
                                        .min = 0,
                                        .max = 6,
                                        .step = 1,
                                        .unit = "",
                                        .si_conversion = "",
                                        .ordered_map = true,
                                        .meaningful_zero = false,
                                        .mapping = &myMapping,
                                        .period = "0",
                                        .delta = "0",
};

void controlModeCallback(Value *value, double data, String timestamp)
{
    myMode = (int)data;
    value->report(myMode);
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
    myNetwork = wappsto.createNetwork("Mode Example");

    // Create device
    myDevice = myNetwork->createDevice(&myDeviceDescription);

    // Create LED value
    myModeValue = myDevice->createNumberValue(&myModeParameters);
    myModeValue->onControl(&controlModeCallback);
}

void loop()
{
    delay(200);
    wappsto.dataAvailable();
}
