#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "time.h"
#include "WappstoIoT.h"
#include "WappstoRpc.h"

#define JSON_CHAR_BUFFER 500
#define JSON_POST_BUFFER 2072

//#define DISABLE_FAST_SENDING 1


WappstoRpc* WappstoRpc::instance()
{
    static WappstoRpc instance_;
    return &instance_;
}

void WappstoRpc::init(WiFiClientSecure *client)
{
    this->_client = client;
    this->_wappstoLog = WappstoLog::instance();
    _msgId = random(0xFFFF);
}

int WappstoRpc::_getNextMsgId(void)
{
    char tmp[20];
    _msgId++;
    return _msgId;
}

bool WappstoRpc::_awaitResponse(void)
{
    uint16_t timeoutCounter = 0;
/*
    if(this->_client->connected()) {
        Serial.println("_awaitResponse CLIENT IS CONNECTED");
    } else {
        Serial.println("_awaitResponse CLIENT IS NOT CONNECTED");
    }
*/
    while (this->_client->connected()) {
        int ret;
        memset(rspBuffer, 0x00, sizeof(rspBuffer));
        ret = this->_client->read(rspBuffer, sizeof(rspBuffer));
        if (ret > 0) {
            //PRINTV("Await response bytes received: ", ret);
            this->_wappstoLog->verbose((const char*)rspBuffer);

            StaticJsonDocument<JSON_CHAR_BUFFER> root;
            DeserializationError err = deserializeJson(root, rspBuffer);

            if(err) {
                this->_wappstoLog->warning("RPC Response Not parsable Json");
                return false;
            }
            int msgId = root["id"];
            if(msgId != _msgId) {
                this->_wappstoLog->warning("Msg id incorrect: ", msgId);
                this->_wappstoLog->warning("Expected: ", _msgId);
                continue;
            }

            bool result = root["result"]["value"];
            if(result == true) {
                this->_wappstoLog->verbose("RPC Response Success");
                return true;
            } else {
                this->_wappstoLog->warning("RPC Response Error");
                return false;
            }
        }
        delay(10);
        timeoutCounter++;
        if(timeoutCounter > 300) {
            this->_wappstoLog->warning("Timeout waiting for reply");
            return false;
        }
    }
}

bool WappstoRpc::_awaitUuidResponse(char *uuid)
{
    uint16_t timeoutCounter = 0;
/*
    if(this->_client->connected()) {
        Serial.println("_awaitUuidResponse CLIENT IS CONNECTED");
    } else {
        Serial.println("_awaitUuidResponse CLIENT IS NOT CONNECTED");
    }
*/
    while (this->_client->connected()) {
        int ret;
        memset(rspBuffer, 0x00, sizeof(rspBuffer));
        ret = this->_client->read(rspBuffer, sizeof(rspBuffer));
        if (ret > 0) {
            //PRINTV("Await UUID bytes received: ", ret);
            this->_wappstoLog->verbose((const char*)rspBuffer);

            StaticJsonDocument<JSON_CHAR_BUFFER> root;
            DeserializationError err = deserializeJson(root, rspBuffer);

            if(err) {
                this->_wappstoLog->warning("RPC Response Not parsable Json");
                return false;
            }
            int msgId = root["id"];
            if(msgId != _msgId) {
                this->_wappstoLog->warning("Msg id incorrect: ", msgId);
                this->_wappstoLog->warning("Expected: ", _msgId);
                continue;
            }

            if(root["result"]["value"]["id"].size() > 0) {
                char getId[UUID_LENGTH] = {0,};
                strcpy(uuid, root["result"]["value"]["id"][0]);

                this->_wappstoLog->verbose("Found UUID: ", uuid);
                return true;
            } else {
                return false;
            }
    }
        delay(10);
        timeoutCounter++;
        if(timeoutCounter > 300) {
            this->_wappstoLog->warning("Timeout waiting for reply");
            return false;
        }
    }
}

bool WappstoRpc::_awaitDataTimeResponse(String &data, String &timestamp)
{
    uint16_t timeoutCounter = 0;
/*
    if(this->_client->connected()) {
        Serial.println("_awaitDataTimeResponse CLIENT IS CONNECTED");
    } else {
        Serial.println("_awaitDataTimeResponse CLIENT IS NOT CONNECTED");
    }
*/
    while (this->_client->connected()) {
        int ret;
        memset(rspBuffer, 0x00, sizeof(rspBuffer));
        ret = this->_client->read(rspBuffer, sizeof(rspBuffer));
        if (ret > 0) {
            //PRINTV("Await UUID bytes received: ", ret);
            this->_wappstoLog->verbose((const char*)rspBuffer);

            StaticJsonDocument<JSON_CHAR_BUFFER> root;
            DeserializationError err = deserializeJson(root, rspBuffer);

            if(err) {
                this->_wappstoLog->warning("RPC Response Not parsable Json");
                return false;
            }
            int msgId = root["id"];
            if(msgId != _msgId) {
                this->_wappstoLog->warning("Msg id incorrect: ", msgId);
                this->_wappstoLog->warning("Expected: ", _msgId);
                continue;
            }
            /*
{
   "jsonrpc":"2.0",
   "id":19589,
   "result":{
      "value":{
         "timestamp":"2021-12-16T10:03:11Z",
         "data":"400",
         "type":"Report",
         "meta":{
            "id":"94e6eeb8-bd85-4413-b27d-342b274bcef8",
            "type":"state",
            "version":"2.0"
         }
      },
      "meta":{
         "server_send_time":"2021-12-16T10:04:49.653171Z"
      }
   }
}
*/
            data = String((const char*)root["result"]["value"]["data"]);
            timestamp = String((const char*)root["result"]["value"]["timestamp"]);

            //PRINTV("DATA: ", data);
            //PRINTV("TIME: ", timestamp);

            return true;
    }
        delay(10);
        timeoutCounter++;
        if(timeoutCounter > 300) {
            this->_wappstoLog->warning("Timeout waiting for reply");
            return false;
        }
    }
}

void WappstoRpc::_sendSuccessResponse(const char *id)
{
    StaticJsonDocument<JSON_CHAR_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_CHAR_BUFFER);

    root["jsonrpc"] = "2.0";
    root["id"] = id;
    root["result"] = true;

    serializeJson(root, _jsonTxBufferChar);
    this->_client->print(_jsonTxBufferChar);
}


bool WappstoRpc::postNetwork(const char *networkId, String &networkName)
{
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
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
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);

    return(_awaitResponse());
}

bool WappstoRpc::postDevice(Device *device)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
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
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);

    return(_awaitResponse());
}

bool WappstoRpc::postValue(Value *value)
{
    char url[200] = {0,};
    sprintf(url, "/network/%s/device/%s/value", &value->parent->parent->uuid, &value->parent->uuid);
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
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
    if(value->permission == READ_WRITE) {
        data["permission"] = "rw";
    } else if(value->permission == READ) {
        data["permission"] = "r";
    } else if(value->permission == WRITE) {
        data["permission"] = "w";
    }

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
    } else if(value->valueType == BLOB_VALUE) {
        JsonObject str = data.createNestedObject("blob");
        str["max"] = value->valBlob->max;
        str["encoding"] = value->valBlob->encoding;
    }

    JsonArray stateArray = data.createNestedArray("state");
    if(value->permission == READ || value->permission == READ_WRITE) {
        if(value->reportState->requiresPost) {
            JsonObject report = stateArray.createNestedObject();
            JsonObject metaValReport = report.createNestedObject("meta");
            metaValReport["version"] = "2.0";
            metaValReport["type"] = "state";
            metaValReport["id"] = value->reportState->uuid;
            report["type"] = "Report";
            report["timestamp"] = getUtcTime();
            if(value->valueType == NUMBER_VALUE) {
                report["data"] = "";
            } else {
                report["data"] = "";
            }
        }
    }

    if(value->permission == WRITE || value->permission == READ_WRITE) {
        if(value->controlState->requiresPost) {
            JsonObject control = stateArray.createNestedObject();
            JsonObject metaValControl = control.createNestedObject("meta");
            metaValControl["version"] = "2.0";
            metaValControl["type"] = "state";
            metaValControl["id"] = value->controlState->uuid;
            control["type"] = "Control";
            control["timestamp"] = getUtcTime();
            if(value->valueType == NUMBER_VALUE) {
                control["data"] = "";
            } else {
                control["data"] = "";
            }
        }
    }

    serializeJson(root, _jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);

    return(_awaitResponse());
}

bool WappstoRpc::deleteValue(const char* networkId, const char* deviceId, int valueId)
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
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);

    return(_awaitResponse());
    */
    return false;
}

bool WappstoRpc::putValue(Value *value)
{
    /*
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
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
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);

    return(_awaitResponse());
    */
    return false;
}

bool WappstoRpc::sendPing(void)
{
    // {"jsonrpc":"2.0","method":"HEAD","id":"PING-12","params":{"url":"/services/2.0/network"}}
/*
    if(this->_client->connected()) {
        Serial.println("sendPing CLIENT IS CONNECTED");
    } else {
        Serial.println("sendPing CLIENT IS NOT CONNECTED");
    }
*/
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
    root["method"] = "HEAD";
    JsonObject params = root.createNestedObject("params");
    params["url"] = "/services/2.0/network";
    serializeJson(root, _jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);
    return(_awaitResponse());
}

bool WappstoRpc::putState(State *state)
{
/*
    if(this->_client->connected()) {
        Serial.println("putState CLIENT IS CONNECTED");
    } else {
        Serial.println("putState CLIENT IS NOT CONNECTED");
    }
*/
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
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
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);
    return(_awaitResponse());
}

RequestType_e WappstoRpc::readData(char* uuid, char *dataPtr, char *timestampPtr)
{
    int ret;
    memset(readBuffer, 0x00, sizeof(readBuffer));
    ret = this->_client->read(readBuffer, sizeof(readBuffer));
    if(ret > 0) {
        this->_wappstoLog->verbose("Reading received:");
        this->_wappstoLog->verbose((const char*)readBuffer);

        StaticJsonDocument<JSON_STATIC_BUFFER_SIZE> root;
        DeserializationError err = deserializeJson(root, readBuffer);

        if (err) {
            this->_wappstoLog->warning("Not parsable Json receive");
            return REQUEST_UNKNOWN;
        }

        this->_wappstoLog->verbose(root);
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

                this->_wappstoLog->verbose("GET: ", getId);
                strcpy(uuid, getId);
                _sendSuccessResponse(msgId);
                return REQUEST_GET;
            } else if(strcmp(method, "PUT") == 0) {
                JsonObject data = params["data"];
                strcpy(dataPtr, data["data"]);
                strcpy(timestampPtr, data["timestamp"]);

                strcpy(uuid, data["meta"]["id"]);

                this->_wappstoLog->verbose("PUT: ",uuid);
                this->_wappstoLog->verbose(dataPtr);

                _sendSuccessResponse(msgId);
                return REQUEST_PUT;
            } else if(strcmp(method, "DELETE") == 0) {
                const char* urlStr = params["url"];
                String url(urlStr);
                char getId[UUID_LENGTH] = {0,};
                int lastSlash = url.lastIndexOf('/');
                url.substring(lastSlash+1).toCharArray(getId, UUID_LENGTH);

                this->_wappstoLog->verbose("DELETE: ", getId);
                strcpy(uuid, getId);
                _sendSuccessResponse(msgId);
                return REQUEST_DELETE;
            } else {
                this->_wappstoLog->verbose("method");
            }
        } else if(root.containsKey("result")) {
            bool result = root["result"]["value"];
            if(result == true) {
                this->_wappstoLog->verbose("Return success");
                return REQUEST_SUCCESS;
            } else {
                this->_wappstoLog->warning("Return Error");
            }
        } else {
            this->_wappstoLog->warning("Received msg not handled");
        }
    } else {
        this->_wappstoLog->warning("No data read");
    }
    return REQUEST_UNKNOWN;
}

bool WappstoRpc::getDeviceUuidFromName(Network *network, String &name, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/services/2.0/network/%s/device?this_name==%s", network->uuid, name.c_str()); // "hest"); //
    params["url"] = url;
    serializeJson(root, _jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);
    return(_awaitUuidResponse(uuid));
}

bool WappstoRpc::getValueUuidFromName(Device *device, String name, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/services/2.0/device/%s/value?this_name==%s", device->uuid, name.c_str());
    params["url"] = url;
    serializeJson(root, _jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);
    return(_awaitUuidResponse(uuid));
}

bool WappstoRpc::getStateUuidFromName(Value *value, StateType_e stateType, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    if(stateType == TYPE_REPORT) {
        sprintf(url, "/services/2.0/value/%s/state?this_type==Report", value->uuid);
    } else {
        sprintf(url, "/services/2.0/value/%s/state?this_type==Control", value->uuid);
    }
    params["url"] = url;
    serializeJson(root, _jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);
    return(_awaitUuidResponse(uuid));
}

bool WappstoRpc::getStateDataTime(const char *stateUuid, String &data, String &timestamp)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = _getNextMsgId();
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/state/%s", stateUuid);
    params["url"] = url;
    serializeJson(root, _jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(_jsonTxBufferChar);
    return(_awaitDataTimeResponse(data, timestamp));
}
