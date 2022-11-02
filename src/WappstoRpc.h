#pragma once

#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "WappstoIoT.h"
#include "WappstoLog.h"

#define JSON_TX_BUFFER_SIZE 2000
#define JSON_STATIC_BUFFER_SIZE 1000
#define DATA_MAX_LENGTH 250
#define JSON_CHAR_BUFFER 500
#define JSON_POST_BUFFER 2072

class WappstoRpc
{
public:
    static WappstoRpc* instance();
    void init(WiFiClientSecure *client);

    bool send();
    bool send(JsonDocument *doc);
    bool send(JsonDocument *doc, bool waitForResponse);
    bool sendRaw(const char *url, const char *data, Timestamp_t timestamp);
    JsonObject generateRPCRequest(const char* method, const char* url, bool data);
    bool sendPing(void);
    RequestType_e readData(char* uuid, JsonDocument &root);
    bool rpcAvailable(void);

private:
    WappstoRpc() = default;
    ~WappstoRpc() {};
    WappstoRpc(const WappstoRpc&) = delete;
    WappstoRpc& operator=(const WappstoRpc&) = delete;

    bool _awaitResponse(void);
    bool _awaitUuidResponse(char *uuid);
    void _sendSuccessResponse(const char *id);
    bool _readJsonAwait(JsonDocument& root);
    int _getNextMsgId(void);

    StaticJsonDocument<JSON_POST_BUFFER> root;
    WappstoLog *_wappstoLog;
    WiFiClientSecure *_client;
    int _msgId;
    uint8_t _readBuffer[JSON_STATIC_BUFFER_SIZE];
    uint8_t _rspBuffer[JSON_STATIC_BUFFER_SIZE];
    char _jsonTxBufferChar[JSON_TX_BUFFER_SIZE];
    uint8_t _tempBuffer[JSON_TX_BUFFER_SIZE];
};
