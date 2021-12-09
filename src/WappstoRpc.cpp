#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "time.h"
#include "WappstoIoT.h"
#include "WappstoRpc.h"

#define JSON_CHAR_BUFFER 500
#define JSON_POST_BUFFER 2072

//#define DISABLE_FAST_SENDING 1

#define PRINT(s)    { if(_jsonDebug) \
                            Serial.println(F(s)); }
#define PRINTV(s,v) {  if(_jsonDebug) \
                            Serial.print(F(s)); Serial.println(v); }
#define PRINTX(s,v) { if(_jsonDebug) \
                            Serial.print(F(s)); Serial.print(F("0x")); Serial.println(v, HEX); }

WappstoRpc::WappstoRpc(WiFiClientSecure *client)
{
    _client = client;
    _jsonDebug = true;
    _msgId = 1;
}

void WappstoRpc::setDebug(bool jsonDebug)
{
    _jsonDebug = jsonDebug;
}

uint8_t WappstoRpc::_awaitResponse(void)
{
    uint16_t timeoutCounter = 0;
    while (_client->connected()) {
        int ret;
        memset(rspBuffer, 0x00, sizeof(rspBuffer));
        ret = _client->read(rspBuffer, sizeof(rspBuffer));
        if (ret > 0) {
            PRINTV("Init received: ", ret);
            PRINT(rspBuffer);

            StaticJsonDocument<JSON_CHAR_BUFFER> root;
            DeserializationError err = deserializeJson(root, rspBuffer);

            if(err) {
                PRINT("RPC Response Not parsable Json");
                return(0);
            }

            bool result = root["result"]["value"];
            if(result == true) {
                PRINT("RPC Response Success");
                return(1);
            } else {
                PRINT("RPC Response Error");
                return(0);
            }
        }
        delay(10);
        timeoutCounter++;
        if(timeoutCounter > 100) {
            PRINT("Timeout waiting for reply");
            return(0);
        }
    }
}

uint8_t WappstoRpc::_awaitUuidResponse(char *uuid)
{
    uint16_t timeoutCounter = 0;
    while (_client->connected()) {
        int ret;
        memset(rspBuffer, 0x00, sizeof(rspBuffer));
        ret = _client->read(rspBuffer, sizeof(rspBuffer));
        if (ret > 0) {
            PRINTV("Init received: ", ret);
            PRINT(rspBuffer);

            StaticJsonDocument<JSON_CHAR_BUFFER> root;
            DeserializationError err = deserializeJson(root, rspBuffer);

            if(err) {
                PRINT("RPC Response Not parsable Json");
                return(0);
            }
            if(root["result"]["value"]["id"].size() > 0) {
                char getId[UUID_LENGTH] = {0,};
                strcpy(uuid, root["result"]["value"]["id"][0]);

                Serial.print("Found UUID: ");
                Serial.println(uuid);
                return(1);
            } else {
                return(0);
            }
    }
        delay(10);
        timeoutCounter++;
        if(timeoutCounter > 100) {
            PRINT("Timeout waiting for reply");
            return(0);
        }
    }
}

void WappstoRpc::_sendSuccessResponse(const char *id)
{
    StaticJsonDocument<JSON_CHAR_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_CHAR_BUFFER);

    root["jsonrpc"] = "2.0";
    root["id"] = id;
    _msgId++;
    root["result"] = true;

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);
}


int WappstoRpc::postNetwork(const char *networkId, String &networkName)
{
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "POST";
    JsonObject params = root.createNestedObject("params");
    params["url"] = "/network";
    JsonObject data = params.createNestedObject("data");
    data["name"] = networkName;
    JsonObject meta = data.createNestedObject("meta");
    meta["id"] = networkId;
    meta["type"] = "network";
    meta["version"] = "2.0";

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);

    return(_awaitResponse());
}

int WappstoRpc::postDevice(Device *device)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "POST";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/network/%s/device", &device->parent->uuid);
    params["url"] = url;

    JsonObject data = params.createNestedObject("data");
    JsonObject metaDev = data.createNestedObject("meta");
    metaDev["id"] = device->uuid;
    metaDev["type"] = "device";
    metaDev["version"] = "2.0";

    data["name"] = device->name;
    data["manufacturer"] = device->deviceInfo.manufacturer;
    data["product"] = device->deviceInfo.product;
    data["version"] = device->deviceInfo.version;
    data["serial"] = device->deviceInfo.serial;
    data["description"] = device->deviceInfo.description;
    data["protocol"] = device->deviceInfo.protocol;
    data["communication"] = device->deviceInfo.communication;

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);

    return(_awaitResponse());
}

int WappstoRpc::postValue(Value *value)
{
    char url[200] = {0,};
    sprintf(url, "/network/%s/device/%s/value", &value->parent->parent->uuid, &value->parent->uuid);
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "POST";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/network/%s/device/%s/value", &value->parent->parent->uuid, &value->parent->uuid);
    params["url"] = url;

    JsonObject data = params.createNestedObject("data");

    JsonObject metaVal = data.createNestedObject("meta");
    metaVal["version"] = "2.0";
    metaVal["type"] = "value";
    metaVal["id"] = value->uuid;

    data["name"] = value->name;
    data["permission"] = "rw";
    data["type"] = value->type;

    if(value->valueType == NUMBER_VALUE) {
        JsonObject number = data.createNestedObject("number");
        number["min"] = value->valNumber->min;
        number["max"] = value->valNumber->max;
        number["step"] = value->valNumber->step;
        number["unit"] = "";
    } else if(value->valueType == STRING_VALUE) {
        JsonObject str = data.createNestedObject("string");
        str["max"] = value->valString->max;
        str["encoding"] = value->valString->encoding;
    }

    JsonArray stateArray = data.createNestedArray("state");
    if(value->permission == READ || value->permission == READ_WRITE) {
        JsonObject report = stateArray.createNestedObject();
        JsonObject metaValReport = report.createNestedObject("meta");
        metaValReport["version"] = "2.0";
        metaValReport["type"] = "state";
        metaValReport["id"] = value->reportState->uuid;
        report["type"] = "Report";
        report["timestamp"] = getUtcTime();
        report["data"] = "";
    }

    if(value->permission == WRITE || value->permission == READ_WRITE) {
        JsonObject control = stateArray.createNestedObject();
        JsonObject metaValControl = control.createNestedObject("meta");
        metaValControl["version"] = "2.0";
        metaValControl["type"] = "state";
        metaValControl["id"] = value->controlState->uuid;
        control["type"] = "Control";
        control["timestamp"] = getUtcTime();
        control["data"] = "";
    }

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);

    return(_awaitResponse());
}

int WappstoRpc::deleteValue(const char* networkId, const char* deviceId, int valueId)
{
    /*
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = 1;
    root["method"] = "DELETE";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/network/%s/device/%s/value/%s", networkId, deviceId, _values[valueId-1].valueUuid);
    params["url"] = url;

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);

    return(_awaitResponse());
    */
    return 0;
}

int WappstoRpc::putValue(Value *value)
{
    /*
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "PUT";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/network/%s/device/%s/value/%s/state/%s", networkId, deviceId, _values[valueId-1].valueUuid, _values[valueId-1].reportUuid);
    params["url"] = url;

    JsonObject data = params.createNestedObject("data");
    JsonObject meta = data.createNestedObject("meta");
    meta["id"] = _values[valueId-1].reportUuid;
    meta["type"] = "state";
    meta["version"] = "2.0";

    data["timestamp"] = getUtcTime();
    data["data"] = _values[valueId-1].data;
    data["type"] = "Report";

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);

    return(_awaitResponse());
    */
    return 0;
}

int WappstoRpc::putState(State *state)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "PUT";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/network/%s/device/%s/value/%s/state/%s", state->parent->parent->parent->uuid, state->parent->parent->uuid, state->parent->uuid, state->uuid);
    params["url"] = url;

#ifndef DISABLE_FAST_SENDING
    JsonObject metaFast = params.createNestedObject("meta");
    metaFast["fast"] = true;
#endif

    JsonObject data = params.createNestedObject("data");
    JsonObject meta = data.createNestedObject("meta");
    meta["id"] = state->uuid;
    meta["type"] = "state";
    meta["version"] = "2.0";

    data["timestamp"] = state->timestamp;
    data["data"] = state->data;
    if(state->stateType == TYPE_REPORT) {
        data["type"] = "Report";
    } else {
        data["type"] = "Control";
    }

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);
    return(_awaitResponse());
}

RequestType_e WappstoRpc::readData(char* uuid, char *dataPtr)
{
    int ret;
    memset(readBuffer, 0x00, sizeof(readBuffer));
    ret = _client->read(readBuffer, sizeof(readBuffer));
    if(ret > 0) {
        PRINT("Reading received:");
        PRINT(readBuffer);

        StaticJsonDocument<JSON_STATIC_BUFFER_SIZE> root;
        DeserializationError err = deserializeJson(root, readBuffer);

        if (err) {
            PRINT("Not parsable Json receive");
            return REQUEST_UNKNOWN;
        }

        if(_jsonDebug) {
            serializeJsonPretty(root, Serial);
        }

        if(root.containsKey("method")) {
            const char *msgId = root["id"];
            const char* method = root["method"];
            JsonObject params = root["params"];

            if(strcmp(method, "GET") == 0) {
                const char* urlStr = params["url"];
                String url(urlStr);
                char getId[UUID_LENGTH] = {0,};
                int lastSlash = url.lastIndexOf('/');
                url.substring(lastSlash+1).toCharArray(getId, UUID_LENGTH);

                PRINTV("GET: ", getId);
                strcpy(uuid, getId);
                _sendSuccessResponse(msgId);
                return REQUEST_GET;
            } else if(strcmp(method, "PUT") == 0) {
                JsonObject data = params["data"];
                strcpy(dataPtr, data["data"]);
                strcpy(uuid, data["meta"]["id"]);

                PRINTV("PUT: ",uuid);
                PRINT(dataPtr);

                _sendSuccessResponse(msgId);
                return REQUEST_PUT;
            } else if(strcmp(method, "DELETE") == 0) {
                const char* urlStr = params["url"];
                String url(urlStr);
                char getId[UUID_LENGTH] = {0,};
                int lastSlash = url.lastIndexOf('/');
                url.substring(lastSlash+1).toCharArray(getId, UUID_LENGTH);

                PRINTV("DELETE: ", getId);
                strcpy(uuid, getId);
                _sendSuccessResponse(msgId);
                return REQUEST_DELETE;
            } else {
                PRINT("method");
            }
        } else if(root.containsKey("result")) {
            bool result = root["result"]["value"];
            if(result == true) {
                PRINT("Return success");
                return REQUEST_SUCCESS;
            } else {
                PRINT("Return Error");
            }
        } else {
            PRINT("Received msg not handled");
        }
    } else {
        PRINT("No data read");
    }
    return REQUEST_UNKNOWN;
}

int WappstoRpc::getDeviceUuidFromName(Network *network, String &name, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/services/2.0/network/%s/device?this_name==%s", network->uuid, name.c_str()); // "hest"); //
    params["url"] = url;
    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);
    return(_awaitUuidResponse(uuid));
}

int WappstoRpc::getValueUuidFromName(Device *device, String name, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/services/2.0/device/%s/value?this_name==%s", device->uuid, name.c_str());
    params["url"] = url;
    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);
    return(_awaitUuidResponse(uuid));
}

int WappstoRpc::getStateUuidFromName(Value *value, StateType_e stateType, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    if(stateType == TYPE_REPORT) {
        sprintf(url, "/services/2.0/value/%s/state?this_type==Report", value->uuid);
    } else {
        sprintf(url, "/services/2.0/value/%s/state?this_type==Control", value->uuid);
    }
    params["url"] = url;
    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);
    return(_awaitUuidResponse(uuid));
}
