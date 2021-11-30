#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "time.h"
#include "WappstoIoT.h"
#include "WappstoRpc.h"

#define JSON_CHAR_BUFFER 500
#define JSON_POST_BUFFER 2072

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

uint8_t WappstoRpc::_awaitResponse()
{
    while (_client->connected()) {
        int ret;
        memset(rspBuffer, 0x00, sizeof(rspBuffer));
        ret = _client->read(rspBuffer, sizeof(rspBuffer));
        if (ret > 0) {
            if(_jsonDebug){
                Serial.println("Init received:");
                String rcvData((char*)&rspBuffer);
                Serial.println(rcvData);
            }
            //StaticJsonDocument<JSON_CHAR_BUFFER> root;
            DynamicJsonDocument root(JSON_CHAR_BUFFER);
            DeserializationError err = deserializeJson(root, rspBuffer);

            if(err) {
                Serial.println("RPC Response Not parsable Json");
                root.clear();
                //root.delete();
                root.~BasicJsonDocument();
                return(0);
            }

            bool result = root["result"]["value"];
            if(result == true) {
                if(_jsonDebug) {
                    Serial.println("RPC Response Success");
                }
                root.clear();
                //root.delete();
                root.~BasicJsonDocument();
                return(1);
            } else {
                Serial.println("RPC Response Error");
                root.clear();
                //root.delete();
                root.~BasicJsonDocument();
                return(0);
            }
        }
    }
}

uint8_t WappstoRpc::_awaitResponseControl(void)
{
    while (_client->connected()) {
        int ret;
        memset(rspBuffer, 0x00, sizeof(rspBuffer));
        ret = _client->read(rspBuffer, sizeof(rspBuffer));
        if (ret > 0) {
            if(_jsonDebug){
                Serial.println("Init received:");
                String rcvData((char*)&rspBuffer);
                Serial.println(rcvData);
            }
            //StaticJsonDocument<JSON_CHAR_BUFFER> root;
            DynamicJsonDocument root(JSON_CHAR_BUFFER);
            DeserializationError err = deserializeJson(root, rspBuffer);

            if(err) {
                Serial.println("RPC Response Not parsable Json");
                return(0);
            }
            JsonObject result = root["result"];
            JsonObject value = result["value"];
            const char* dataStr = root["result"]["value"]["data"];
            const char* uuidStr = root["result"]["value"]["meta"]["id"];

            Serial.println("TEST");
            Serial.println(dataStr);
            Serial.println(uuidStr);
#if 0

            breaks stuff!!!
            /*
            if(strlen(dataStr) > 0) {
                for(int i=1; i<=20; i++) {
                    if(strcmp(uuidStr, _values[i-1].controlUuid) == 0) {
                        Serial.println("FOUND STUFF");
                        strcpy(_values[i-1].ctrlData, dataStr);
                        return i;
                    }
                }
            }
            */
#endif
        }
    }
    return 0;
}


void WappstoRpc::_sendSuccessResponse(const char *id)
{
    //StaticJsonDocument<JSON_CHAR_BUFFER> root;
DynamicJsonDocument root(JSON_CHAR_BUFFER);
    memset(_jsonTxBufferChar, 0x00, JSON_CHAR_BUFFER);
    //char _jsonTxBufferChar[JSON_TX_BUFFER_SIZE];

    root["jsonrpc"] = "2.0";
    root["id"] = id;
    _msgId++;
    root["result"] = true;

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);
    root.clear();
    //root.delete();
    root.~BasicJsonDocument();
}


int WappstoRpc::postNetwork(const char *networkId, String &networkName)
{
    //StaticJsonDocument<JSON_POST_BUFFER> root;
    DynamicJsonDocument root(JSON_CHAR_BUFFER);
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);
    //char _jsonTxBufferChar[JSON_TX_BUFFER_SIZE];

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
    root.clear();
    //root.delete();
    root.~BasicJsonDocument();

    return(_awaitResponse());
}

int WappstoRpc::postDevice(Device *device)
{
    char url[200] = {0,};
    //StaticJsonDocument<JSON_POST_BUFFER> root;
    DynamicJsonDocument root(JSON_CHAR_BUFFER);
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);
    //char _jsonTxBufferChar[JSON_TX_BUFFER_SIZE];

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
    data["included"] = "1";

    serializeJson(root, _jsonTxBufferChar);
    if(_jsonDebug) {
        serializeJsonPretty(root, Serial);
    }
    _client->print(_jsonTxBufferChar);
    root.clear();
    //root.delete();
    root.~BasicJsonDocument();

    return(_awaitResponse());
}

int WappstoRpc::postValue(Value *value)
{
    char url[200] = {0,};
    sprintf(url, "/network/%s/device/%s/value", &value->parent->parent->uuid, &value->parent->uuid);
    Serial.println(url);
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

    JsonObject report = stateArray.createNestedObject();
    JsonObject metaValReport = report.createNestedObject("meta");
    metaValReport["version"] = "2.0";
    metaValReport["type"] = "state";
    metaValReport["id"] = value->reportState->uuid;
    report["type"] = "Report";
    report["timestamp"] = getUtcTime();
    report["data"] = "";

    JsonObject control = stateArray.createNestedObject();
    JsonObject metaValControl = control.createNestedObject("meta");
    metaValControl["version"] = "2.0";
    metaValControl["type"] = "state";
    metaValControl["id"] = value->controlState->uuid;
    control["type"] = "Control";
    control["timestamp"] = getUtcTime();
    control["data"] = "";

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

int WappstoRpc::deleteAllValues(const char* networkId, const char* deviceId)
{
    /*
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _msgId;
    _msgId++;
    root["method"] = "DELETE";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/network/%s/device/%s/value", networkId, deviceId);
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
        if(_jsonDebug) {
            Serial.println("Reading received:");
            String readData((char*)&readBuffer);
            Serial.println(readData);
        }

        StaticJsonDocument<JSON_STATIC_BUFFER_SIZE> root;
        DeserializationError err = deserializeJson(root, readBuffer);

        if (err) {
            Serial.println("Not parsable Json receive");
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

                if(_jsonDebug) {
                    Serial.print("GET: ");
                    Serial.println(getId);
                }
                strcpy(uuid, getId);
                _sendSuccessResponse(msgId);
                return REQUEST_GET;
            } else if(strcmp(method, "PUT") == 0) {
                JsonObject data = params["data"];
                strcpy(dataPtr, data["data"]);
                strcpy(uuid, data["meta"]["id"]);

                if(_jsonDebug) {
                    Serial.print("PUT: ");
                    Serial.print(uuid);
                    Serial.print(" - ");
                    Serial.println(dataPtr);
                }
                _sendSuccessResponse(msgId);
                return REQUEST_PUT;
            } else {
                if(_jsonDebug) {
                    Serial.println("Unknown method");
                }
            }
        } else if(root.containsKey("result")) {
            bool result = root["result"]["value"];
            if(result == true) {
                if(_jsonDebug) {
                    Serial.println("Return success");
                    return REQUEST_SUCCESS;
                }
            } else {
                if(_jsonDebug) {
                    Serial.println("Return Error");
                }
            }
        } else {
            if(_jsonDebug) {
                Serial.println("Received msg not handled");
            }
        }
    } else {
        if(_jsonDebug) {
            Serial.println("No data read");
        }
    }
    return REQUEST_UNKNOWN;
}
