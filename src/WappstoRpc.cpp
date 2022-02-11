#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "time.h"
#include "WappstoIoT.h"
#include "WappstoRpc.h"


//#define DISABLE_FAST_SENDING 1


bool convertToJson(const String& s, JsonVariant& variant) {
  return variant.set(s.c_str());
}

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
    if(!_client->connected()) {
        this->_wappstoLog->error("Cannot send, not connected");
        return false;
    }
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

#ifndef DISABLE_FAST_SENDING
    if(strcmp(method, "GET") != 0) {
        JsonObject metaFast = params.createNestedObject("meta");
        metaFast["fast"] = true;
    }
#endif

    if(data) {
        return params.createNestedObject("data");
    } else {
        return JsonObject();
    }
}

bool WappstoRpc::_readJsonAwait(JsonDocument& root)
{
    uint16_t timeoutCounter = 0;

    while (this->_client->connected()) {
        int ret;
        memset(this->_rspBuffer, 0x00, sizeof(this->_rspBuffer));
        ret = this->_client->read(this->_rspBuffer, sizeof(this->_rspBuffer) - 1);
        if(ret < 0) {
            this->_wappstoLog->error("Read return error");
            return false;
        }
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
