/** Generic example for thermostat

To run on a target see the examples for ESP32 and WIO terminal.

**/

#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include "Wappsto.h"
#include "wappsto_config.h"

WiFiMulti WiFiMulti;
WiFiClientSecure client;

const char* ssid = "";
const char* password = "";

double temperatureReading = 18.0;
double temperatureTarget = 21.0;

unsigned long startMillis = 0;
unsigned long currentMillis = 0;
#define UPDATE_INTERVAL_MS 1000 // 1 second

Wappsto wappsto(&client);
Network *network;
Device *thermostatDevice;
Value *temperatureValue;

DeviceDescription_t thermostatParameters = {
    .name = "Thermostat",
    .product = "",
    .manufacturer = "",
    .description = "",
    .version = "",
    .serial = "",
    .protocol = "",
    .communication = "",
};

ValueNumber_t temperatureParameters = {
    .name = "Temperature",
    .type = "temperature",
    .permission = READ_WRITE,
    .min = -30,
    .max = 50,
    .step = 1,
    .unit = "°C",
    .si_conversion = "[K] = [°C] + 273.15"
};

void refreshTemperatureCallback(Value *value)
{
    value->report((int)temperatureReading);
}

void controlTemperatureCallback(Value *value, double data, String timestamp)
{
    temperatureTarget = data;
}

void createThermostat(void)
{
    // Create network
    network = wappsto.createNetwork("Home");

    // Create device
    thermostatDevice = network->createDevice(&thermostatParameters);

    // Create temperature value
    temperatureValue = thermostatDevice->createNumberValue(&temperatureParameters);
    temperatureValue->onRefresh(&refreshTemperatureCallback);
    temperatureValue->onControl(&controlTemperatureCallback);
    temperatureTarget = temperatureValue->getControlData().toDouble();
    temperatureValue->report((int)temperatureReading);
}

void updateTemperature(void)
{
    double diffToTarget = temperatureTarget - temperatureReading;
    Serial.println(abs(diffToTarget));
    if(abs(diffToTarget) >= 1) {
        if(abs(diffToTarget) <= 2) {
            temperatureReading = temperatureTarget;
        } else {
            temperatureReading += (diffToTarget / 3);
        }
        temperatureValue->report((int)temperatureReading);
    }
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

    createThermostat();
}

void loop()
{
    delay(500);
    wappsto.dataAvailable();
    currentMillis = millis();
    if(currentMillis - startMillis >= UPDATE_INTERVAL_MS) {
        startMillis = currentMillis;
        updateTemperature();
    }
}
