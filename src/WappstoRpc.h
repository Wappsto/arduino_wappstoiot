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
        static WappstoRpc* instance();
        void init(WiFiClientSecure *client);
        bool postNetwork(const char *networkId, String &networkName);
        bool postDevice(Device *device);
        bool postValue(Value *value);
        bool putValue(Value *value);
        bool putState(State *state);
        bool deleteValue(const char* networkId, const char* deviceId, int valueId);
        bool getDeviceUuidFromName(Network *network, String &name, char *uuid);
        bool getValueUuidFromName(Device *device, String name, char *uuid);
        bool getStateUuidFromName(Value *value, StateType_e stateType, char *uuid);
        bool getStateDataTime(const char *stateUuid, String &data, String &timestamp);
        bool sendPing(void);

        RequestType_e readData(char* uuid, char* data, char *timestampPtr);

    private:
        WappstoRpc() = default;
        ~WappstoRpc() {};
        WappstoRpc(const WappstoRpc&) = delete;
        WappstoRpc& operator=(const WappstoRpc&) = delete;

        WappstoLog *_wappstoLog;
        WiFiClientSecure *_client;
        int _msgId;
        int _getNextMsgId(void);
        bool _jsonDebug;
        uint8_t readBuffer[JSON_STATIC_BUFFER_SIZE];
        uint8_t rspBuffer[JSON_STATIC_BUFFER_SIZE];
        char _jsonTxBufferChar[JSON_TX_BUFFER_SIZE];
        bool _awaitResponse(void);
        bool _awaitUuidResponse(char *uuid);
        bool _awaitDataTimeResponse(String &data, String &timestamp);
        void _sendSuccessResponse(const char *id);
        const char* _getUtcTime(void);
};
