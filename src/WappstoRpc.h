#pragma once

#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "WappstoIoT.h"
//#include "Device.h"

#define JSON_TX_BUFFER_SIZE 2000
#define JSON_STATIC_BUFFER_SIZE 600
#define DATA_MAX_LENGTH 250

class WappstoRpc
{
    public:
        WappstoRpc(WiFiClientSecure *client);
        void setDebug(bool jsonDebug);
        int postNetwork(const char *networkId, String &networkName);
        int postDevice(Device *device);
        int postValue(Value *value);
        int putValue(Value *value);
        int putState(State *state);
        int deleteValue(const char* networkId, const char* deviceId, int valueId);
        RequestType_e readData(char* uuid, char* data);

    private:
        int _msgId;
        WiFiClientSecure *_client;
        bool _jsonDebug;
        uint8_t readBuffer[JSON_STATIC_BUFFER_SIZE];
        uint8_t rspBuffer[JSON_STATIC_BUFFER_SIZE];
        char _jsonTxBufferChar[JSON_TX_BUFFER_SIZE];
        uint8_t _awaitResponse(void);
        void _sendSuccessResponse(const char *id);
        const char* _getUtcTime(void);
};
