#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "time.h"
#include "WappstoIoT.h"
#include "WappstoRpc.h"


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
    this->_msgId = random(0xFFFF);
}

int WappstoRpc::_getNextMsgId(void)
{
    return ++this->_msgId;
}

bool WappstoRpc::send(JsonDocument *doc, bool waitForResponse)
{
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);
    serializeJson(this->root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(this->root);
    this->_client->print(this->_jsonTxBufferChar);

    if(!waitForResponse) {
        return true;
    }

    if(doc) {
        this->root.clear();
        return this->_readJsonAwait(*doc);
    }

    return(this->_awaitResponse());
}

bool WappstoRpc::send(JsonDocument *doc)
{
    return this->send(doc, true);
}

bool WappstoRpc::send()
{
    return this->send(NULL, true);
}

JsonObject WappstoRpc::generateRPCRequest(const char* method, const char* url, bool data)
{
    this->root.clear();

    this->root["jsonrpc"] = "2.0";
    this->root["id"] = this->_getNextMsgId();
    this->root["method"] = method;
    JsonObject params = this->root.createNestedObject("params");
    params["url"] = url;

    if(data) {
        return params.createNestedObject("data");
    } else {
        return JsonObject();
    }
}

bool WappstoRpc::_readJsonAwait(JsonDocument& root)
{
    uint16_t timeoutCounter = 0;
/*
    if(this->_client->connected()) {
        Serial.println("_readJsonAwait CLIENT IS CONNECTED");
    } else {
        Serial.println("_readJsonAwait CLIENT IS NOT CONNECTED");
    }
*/
    while (this->_client->connected()) {
        int ret;
        memset(this->_rspBuffer, 0x00, sizeof(this->_rspBuffer));
        ret = this->_client->read(this->_rspBuffer, sizeof(this->_rspBuffer) - 1);
        if (ret > 0) {
            this->_wappstoLog->verbose("Read bytes: ", ret);
            this->_wappstoLog->verbose((const char*)this->_rspBuffer);
            DeserializationError err = deserializeJson(root, this->_rspBuffer);

            if(err) {
                this->_wappstoLog->warning("RPC Response Not parsable Json: ", err.c_str());
                return false;
            }

            int msgId = root["id"];
            if(msgId != this->_msgId) {
                this->_wappstoLog->warning("Msg id incorrect: ", msgId);
                this->_wappstoLog->warning("Expected: ", this->_msgId);
                continue;
            }

            return true;
        }
        delay(10);
        timeoutCounter++;
        if(timeoutCounter > 3000) {
            this->_wappstoLog->warning("Timeout waiting for reply: ", this->_msgId);
            return false;
        }
    }

    return false;
}

bool WappstoRpc::_awaitResponse(void)
{
    StaticJsonDocument<JSON_CHAR_BUFFER> root;
    if(this->_readJsonAwait(root)) {
        bool result = root["result"]["value"];
        if(result == true) {
            this->_wappstoLog->verbose("RPC Response Success");
            return true;
        }
        this->_wappstoLog->warning("RPC Response Error");
    }
    return false;
}

bool WappstoRpc::_awaitUuidResponse(char *uuid)
{
    StaticJsonDocument<JSON_POST_BUFFER> root;
    if(this->_readJsonAwait(root)) {
        if(root["result"]["value"]["id"].size() > 0) {
            strcpy(uuid, root["result"]["value"]["id"][0]);

            this->_wappstoLog->verbose("Found UUID: ", uuid);
            return true;
        }
    }
    return false;
}

bool WappstoRpc::sendPing(void)
{
    this->generateRPCRequest("HEAD", "/services/2.0/network", false);
    return this->send();
}


RequestType_e WappstoRpc::readData(char* uuid, JsonDocument& doc)
{
    int ret;
    memset(this->_readBuffer, 0x00, sizeof(this->_readBuffer));
    ret = this->_client->read(this->_readBuffer, sizeof(this->_readBuffer));
    if(ret <= 0) {
        this->_wappstoLog->warning("No data read");
        return REQUEST_UNKNOWN;
    }
    this->_wappstoLog->verbose("Reading received:");
    this->_wappstoLog->verbose((const char*)this->_readBuffer);

    StaticJsonDocument<JSON_STATIC_BUFFER_SIZE> root;
    DeserializationError err = deserializeJson(root, this->_readBuffer);

    if (err) {
        this->_wappstoLog->warning("Not parsable Json receive");
        return REQUEST_UNKNOWN;
    }

    this->_wappstoLog->verbose(root);
    if(root.containsKey("method")) {
        const char *msgId = root["id"];
        const char* method = root["method"];
        JsonObject params = root["params"];
        const char* urlStr = params["url"];
        String url(urlStr);
        char getId[UUID_LENGTH] = {0,};
        int lastSlash = url.lastIndexOf('/');
        url.substring(lastSlash+1).toCharArray(getId, UUID_LENGTH);

        strcpy(uuid, getId);

        this->_wappstoLog->verbose("RPC Method: ", method);
        this->_wappstoLog->verbose("RPC ID: ", getId);
        this->_sendSuccessResponse(msgId);

        if(strcmp(method, "GET") == 0) {
            return REQUEST_GET;
        } else if(strcmp(method, "PUT") == 0) {
            doc.set(params["data"]);
            this->_wappstoLog->verbose(doc);
            return REQUEST_PUT;
        } else if(strcmp(method, "DELETE") == 0) {
            return REQUEST_DELETE;
        } else {
            this->_wappstoLog->verbose(method);
        }
    } else if(root.containsKey("result")) {
        bool result = root["result"]["value"];
        if(result == true) {
            this->_wappstoLog->verbose("Return success");
            return REQUEST_SUCCESS;
        } else {
            this->_wappstoLog->warning("Return Error");
        }
    }
    this->_wappstoLog->warning("Received msg not handled");
    return REQUEST_UNKNOWN;
}

void WappstoRpc::_sendSuccessResponse(const char *id)
{
    this->root.clear();

    this->root["jsonrpc"] = "2.0";
    this->root["id"] = id;
    this->root["result"] = true;

    this->send(NULL, false);
}

/*
bool WappstoRpc::_awaitDataTimeResponse(String &data, Timestamp_t timestamp)
{
    StaticJsonDocument<JSON_POST_BUFFER> root;
    if(this->_readJsonAwait(root)) {
        data = String((const char*)root["result"]["value"]["data"]);
        strcpy(timestamp, root["result"]["value"]["timestamp"]);
        return true;
    }
    return false;
}



bool WappstoRpc::postNetwork(const char *networkId, String &networkName)
{
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
    root["method"] = "POST";
    JsonObject params = root.createNestedObject("params");
    params["url"] = "/network";
    JsonObject data = params.createNestedObject("data");
    data["name"] = networkName;
    JsonObject meta = data.createNestedObject("meta");
    meta["id"] = networkId;
    meta["type"] = "network";
    meta["version"] = "2.0";

    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);

    return(this->_awaitResponse());
}

bool WappstoRpc::postDevice(Device *device)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
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

    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);

    return(this->_awaitResponse());
}

bool WappstoRpc::postValue(Value *value)
{
    char url[200] = {0,};
    sprintf(url, "/device/%s/value", &value->parent->uuid);
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
    root["method"] = "POST";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/device/%s/value", &value->parent->uuid);
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
        JsonObject blob = data.createNestedObject("blob");
        blob["max"] = value->valBlob->max;
        blob["encoding"] = value->valBlob->encoding;
    } else if(value->valueType == XML_VALUE) {
        JsonObject xml = data.createNestedObject("xml");
        xml["xsd"] = value->valXml->xsd;
        xml["namespace"] = value->valXml->xml_namespace;
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
            report["data"] = "";
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
            control["data"] = "";
        }
    }

    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);

    return(this->_awaitResponse());
}

bool WappstoRpc::deleteValue(const char* networkId, const char* deviceId, int valueId)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = 1;
    root["method"] = "DELETE";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/network/%s/device/%s/value/%s", networkId, deviceId, _values[valueId-1].valueUuid);
    params["url"] = url;

    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);

    return(this->_awaitResponse());

    return false;
}

bool WappstoRpc::putValue(Value *value)
{

    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
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

    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);

    return(this->_awaitResponse());

    return false;
}


bool WappstoRpc::putState(State *state)
{

    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
    root["method"] = "PUT";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/state/%s", state->uuid);
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

    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);
    return(this->_awaitResponse());
}



bool WappstoRpc::getDeviceUuidFromName(WappstoModel *network, String &name, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/network/%s/device?this_name==%s", network->uuid, name.c_str());
    params["url"] = url;
    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);
    return(this->_awaitUuidResponse(uuid));
}

bool WappstoRpc::getValueUuidFromName(WappstoModel *device, String name, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/device/%s/value?this_name==%s", device->uuid, name.c_str());
    params["url"] = url;
    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);
    return(this->_awaitUuidResponse(uuid));
}

bool WappstoRpc::getStateUuidFromName(Value *value, StateType_e stateType, char *uuid)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    if(stateType == TYPE_REPORT) {
        sprintf(url, "/value/%s/state?this_type==Report", value->uuid);
    } else {
        sprintf(url, "/value/%s/state?this_type==Control", value->uuid);
    }
    params["url"] = url;
    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);
    return(this->_awaitUuidResponse(uuid));
}

bool WappstoRpc::getStateDataTime(const char *stateUuid, String &data, Timestamp_t timestamp)
{
    char url[200] = {0,};
    StaticJsonDocument<JSON_POST_BUFFER> root;
    memset(this->_jsonTxBufferChar, 0x00, JSON_TX_BUFFER_SIZE);

    root["jsonrpc"] = "2.0";
    root["id"] = this->_getNextMsgId();
    root["method"] = "GET";
    JsonObject params = root.createNestedObject("params");
    sprintf(url, "/state/%s", stateUuid);
    params["url"] = url;
    serializeJson(root, this->_jsonTxBufferChar);
    this->_wappstoLog->verbose(root);
    this->_client->print(this->_jsonTxBufferChar);
    return(this->_awaitDataTimeResponse(data, timestamp));
}
*/
