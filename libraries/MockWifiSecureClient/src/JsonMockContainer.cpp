#include "JsonMockContainer.h"

#define MOCK_JSON_BUFFER 2048
#define UUID_LENGTH 37

// EpoxyDuino/cores/epoxy/Print.cpp change #define PRINTF_BUFFER_SIZE 250 to higher number for full debug
//#define MOCK_PRINTF(...) Serial.print("\e[1;36m[MOCK] "); Serial.printf(__VA_ARGS__); Serial.print("\e[1;37m");
#define MOCK_PRINTF(...)

JsonMockContainer::JsonMockContainer()
{
    this->_clearAll();
}

JsonMockContainer::~JsonMockContainer()
{
}

void JsonMockContainer::addDeviceUuid(const char* uuid)
{
    this->_deviceUuidList[this->_numberDevices] = (char*)uuid;
    this->_numberDevices++;
}

void JsonMockContainer::addValueUuid(const char* uuid)
{
    this->_valueUuidList[this->_numberValues] = (char*)uuid;
    this->_numberValues++;
}

void JsonMockContainer::addStateUuid(const char* uuid, const char* data, bool isReport)
{
    if(isReport) {
        this->_stateReportUuidList[this->_numberReportStates] = (char*)uuid;
        this->_stateReportDataList[this->_numberReportStates] = (char*)data;
        this->_numberReportStates++;
    } else {
        this->_stateControlUuidList[this->_numberControlStates] = (char*)uuid;
        this->_stateControlDataList[this->_numberControlStates] = (char*)data;
        this->_numberControlStates++;
    }
}

char* JsonMockContainer::_getNextDeviceUuid(void)
{
    if(this->_currentDevice >= this->_numberDevices) {
        MOCK_PRINTF("JsonMockContainer!!!: No more devices in list\n");
        return NULL;
    }
    this->_currentDevice++;
    return this->_deviceUuidList[this->_currentDevice-1];
}

char* JsonMockContainer::_getNextValueUuid(void)
{
    if(this->_currentValue >= this->_numberValues) {
        MOCK_PRINTF("JsonMockContainer!!!: No more values in list\n");
        return NULL;
    }
    this->_currentValue++;
    return this->_valueUuidList[this->_currentValue-1];
}

char* JsonMockContainer::_getNextReportStateUuid(void)
{
    if(this->_currentReportState >= this->_numberReportStates) {
        MOCK_PRINTF("JsonMockContainer!!!: No more Reports in list\n");
        return NULL;
    }
    this->_currentReportState++;
    return this->_stateReportUuidList[this->_currentReportState-1];
}

char* JsonMockContainer::_getNextControlStateUuid(void)
{
    if(this->_currentControlState >= this->_numberControlStates) {
        MOCK_PRINTF("JsonMockContainer!!!: No more Control in list\n");
        return NULL;
    }
    this->_currentControlState++;
    return this->_stateControlUuidList[this->_currentControlState-1];
}

bool JsonMockContainer::_verfifyStateId(const char* urlStr)
{
    String url(urlStr);
    char uuid[UUID_LENGTH] = {0,};
    int lastSlash = url.lastIndexOf('/');
    url.substring(lastSlash+1).toCharArray(uuid, UUID_LENGTH);

    for(int i=0; i < this->_numberReportStates; i++) {
        if(strcmp(uuid, this->_stateReportUuidList[i]) == 0) {
            MOCK_PRINTF("Found Report: %s - [%d]\n", uuid, i);
            return true;
        }
    }

    for(int i=0; i < this->_numberControlStates; i++) {
        if(strcmp(uuid, this->_stateControlUuidList[i]) == 0) {
            MOCK_PRINTF("Found Control: %s - [%d]\n", uuid, i);
            return true;
        }
    }
    return false;
}

void JsonMockContainer::_sendResponse(int msgId, char* returnBuffer, bool success)
{
    StaticJsonDocument<500> response;
    response["jsonrpc"] = "2.0";
    response["id"] = msgId;
    JsonObject result = response.createNestedObject("result");
    result["value"] = success;
    JsonObject meta = result.createNestedObject("meta");
    meta["server_send_time"] = "2022-02-14T10:21:58.207729Z";
    char buffer[2000] = {0,};
    serializeJson(response, buffer);
    strcpy(returnBuffer, buffer);
}

void JsonMockContainer::_getDeviceResponse(const char* state, int msgId, char* uuid, char* returnBuffer, bool success)
{
    StaticJsonDocument<1000> response;
    response["jsonrpc"] = "2.0";
    response["id"] = msgId;
    JsonObject result = response.createNestedObject("result");
    JsonObject value = result.createNestedObject("value");
    value["more"] = false;
    value["limit"] = 1000;

    JsonObject valueMeta = value.createNestedObject("meta");
    valueMeta["type"] = "idlist";
    valueMeta["version"] = "2.0";

    JsonObject meta = result.createNestedObject("meta");
    meta["server_send_time"] = "2022-02-14T10:21:58.207729Z";

    JsonArray child = value.createNestedArray("child");
    JsonArray id = value.createNestedArray("id");

    if(success) {
        value["count"] = 1;
        JsonObject local = child.createNestedObject();
        local["type"] = state;
        local["version"] = "2.0";
        id.add(uuid);
    } else {
        value["count"] = 0;
    }

    char buffer[2000] = {0,};

    serializeJson(response, buffer);
    MOCK_PRINTF("device response: %s\n", buffer);
    strcpy(returnBuffer, buffer);
}

void JsonMockContainer::_sendStateData(int msgId, const char* uuid, bool isReport, const char* data, char* returnBuffer)
{
    StaticJsonDocument<1000> response;
    response["jsonrpc"] = "2.0";
    response["id"] = msgId;
    JsonObject result = response.createNestedObject("result");
    JsonObject value = result.createNestedObject("value");
    value["timestamp"] = "2022-02-14T10:18:16Z";
    value["data"] = data;
    if(isReport) {
        value["type"] = "Report";
    } else {
        value["type"] = "Control";
        value["status"] = "Send";
    }
    JsonObject valueMeta = value.createNestedObject("meta");
    valueMeta["id"] = uuid;
    valueMeta["type"] = "state";
    valueMeta["version"] = "2.0";

    JsonObject meta = result.createNestedObject("meta");
    meta["server_send_time"] = "2022-02-14T10:21:58.207729Z";

    char buffer[2000] = {0,};
    serializeJson(response, buffer);
    MOCK_PRINTF("state data: %s\n", buffer);
    strcpy(returnBuffer, buffer);
}

void JsonMockContainer::_findStateAndSendResponse(int msgId, const char* urlStr, char* returnBuffer)
{
    int foundIndex = -1;
    String url(urlStr);
    char uuid[UUID_LENGTH] = {0,};
    int lastSlash = url.lastIndexOf('/');
    url.substring(lastSlash+1).toCharArray(uuid, UUID_LENGTH);

    for(int i=0; i < this->_numberReportStates; i++) {
        if(strcmp(uuid, this->_stateReportUuidList[i]) == 0) {
            MOCK_PRINTF("Found Report: %s - [%d]\n", uuid, i);
            foundIndex = i;
            this->_sendStateData(msgId, uuid, true, this->_stateReportDataList[i], returnBuffer);
            break;
        }
    }

    if(foundIndex < 0) {
        for(int i=0; i < this->_numberControlStates; i++) {
            if(strcmp(uuid, this->_stateControlUuidList[i]) == 0) {
                MOCK_PRINTF("Found Control: %s - [%d]\n", uuid, i);
                foundIndex = i;
                this->_sendStateData(msgId, uuid, false, this->_stateControlDataList[i], returnBuffer);
                break;
            }
        }
    }
}

void JsonMockContainer::testRefresh(const char* uuid, const char* url, char* returnBuffer)
{
    StaticJsonDocument<1000> response;
    response["jsonrpc"] = "2.0";
    response["id"] = "TestRefresh";
    response["method"] = "GET";
    JsonObject params = response.createNestedObject("params");

    char tmp[250] = {0,};
    sprintf(tmp, "%s%s", url, uuid);
    params["url"] = tmp;

    JsonObject meta = params.createNestedObject("meta");
    meta["identifier"] = "C4n7BaBf4b";

    char buffer[2000] = {0,};
    serializeJson(response, buffer);
    MOCK_PRINTF("refresh: %s\n", buffer);
    strcpy(returnBuffer, buffer);
}

void JsonMockContainer::testControl(const char* uuid, const char* url, const char* data, char* returnBuffer)
{
    StaticJsonDocument<1000> response;
    response["jsonrpc"] = "2.0";
    response["id"] = "TestControl";
    response["method"] = "PUT";
    JsonObject params = response.createNestedObject("params");
    char tmp[250] = {0,};
    sprintf(tmp, "%s%s", url, uuid);
    params["url"] = tmp;

    JsonObject pdata = params.createNestedObject("data");
    pdata["data"] = data;
    pdata["timestamp"] = "2022-02-16T09:30:18.377000Z";

    JsonObject metaData = pdata.createNestedObject("meta");
    metaData["id"] = uuid;
    metaData["version"] = "2.0";
    metaData["type"] = "state";

    JsonObject meta = params.createNestedObject("meta");
    meta["identifier"] = "C4n7BaBf4b";

    char buffer[2000] = {0,};
    serializeJson(response, buffer);
    MOCK_PRINTF("control: %s\n", buffer);
    strcpy(returnBuffer, buffer);
}

void JsonMockContainer::testDelete(const char* url, char* returnBuffer)
{
/*
{
  "jsonrpc": "2.0",
  "id": "9445be821c10b8baf0ef1e39d7ee1399",
  "method": "DELETE",
  "params": {
    "url": "/network/e3e395db-2a13-4416-ae2a-dd27d7ec42b8"
  }
}

{
  "jsonrpc": "2.0",
  "id": "fc4349f17faac0a63f2926fac6845de0",
  "method": "DELETE",
  "params": {
    "url": "/network/e3e395db-2a13-4416-ae2a-dd27d7ec42b8/device/8b1b2621-af40-4b02-00bd-551537752434",
    "meta": {
      "identifier": "t1jE1317sy"
    }
  }
}
*/

    StaticJsonDocument<1000> response;
    response["jsonrpc"] = "2.0";
    response["id"] = "TestControl";
    response["method"] = "DELETE";
    JsonObject params = response.createNestedObject("params");
    params["url"] = url;

    char buffer[2000] = {0,};
    serializeJson(response, buffer);
    MOCK_PRINTF("delete: %s\n", buffer);
    strcpy(returnBuffer, buffer);
}

bool JsonMockContainer::receiveData(const char* data, char* returnBuffer)
{
    StaticJsonDocument<MOCK_JSON_BUFFER> root;
    MOCK_PRINTF("Receive data\n");

    DeserializationError err = deserializeJson(root, data);
    if(err) {
        MOCK_PRINTF("Mock Invalid Json: %s\n", err.c_str());
        return false;
    }

    if(root.containsKey("method")) {

        int msgId = root["id"];
        const char* method = root["method"];

        if(strcmp(method, "GET") == 0) {
            MOCK_PRINTF("receive data: GET\n");

            JsonObject params = root["params"];
            const char* urlStr = params["url"];
            char *foundUuid = NULL;
            char typeStr[10] = {0,};
            if(strstr(urlStr, "device?") != NULL) {
                foundUuid = this->_getNextDeviceUuid();
                strcpy(typeStr, "device");
            } else if(strstr(urlStr, "value?") != NULL) {
                foundUuid = this->_getNextValueUuid();
                strcpy(typeStr, "value");
            } else if(strstr(urlStr, "state?this_type==Report") != NULL) {
                foundUuid = this->_getNextReportStateUuid();
                strcpy(typeStr, "state");
            } else if(strstr(urlStr, "state?this_type==Control") != NULL) {
                foundUuid = this->_getNextControlStateUuid();
                strcpy(typeStr, "state");
            } else if(strstr(urlStr, "/state/") != NULL) {
                this->_findStateAndSendResponse(msgId, urlStr, returnBuffer);
                return true;
            } else {
                MOCK_PRINTF("Unhandled GET!!! - %s\n", data);
            }


            if(foundUuid) {
                this->_getDeviceResponse(typeStr, msgId, foundUuid, returnBuffer, true);
            } else {
                this->_getDeviceResponse(typeStr, msgId, foundUuid, returnBuffer, false);
            }
        } else if(strcmp(method, "PUT") == 0) {
            MOCK_PRINTF("receive data: PUT\n");
            JsonObject params = root["params"];
            const char* urlStr = params["url"];
            bool uuidInList = this->_verfifyStateId(urlStr);

            /* report from application
            {
              "jsonrpc": "2.0",
              "id": 4477,
              "method": "PUT",
              "params": {
                "url": "/state/076a1128-223d-4b19-8a89-e0ed632ecc8b",
                "meta": {
                  "fast": true
                },
                "data": {
                  "timestamp": "2022-02-16T09:30:18Z",
                  "data": "1",
                  "type": "Report"
                }
              }
            }
            */

            this->_sendResponse(msgId, returnBuffer, uuidInList);
        } else if(strcmp(method, "POST") == 0) {
            MOCK_PRINTF("receive data: POST\n");
            this->_sendResponse(msgId, returnBuffer, true);
        } else if(strcmp(method, "DELETE") == 0) {
            MOCK_PRINTF("receive data: DELETE\n");
            this->_sendResponse(msgId, returnBuffer, true);
        } else {
            MOCK_PRINTF("UNKNOWN!!!!\n");
        }

        return true;

    } else if(root.containsKey("result")) {
        bool result = false;
        if(root.containsKey("value")) {
            result = root["result"]["value"];
        } else {
            result = root["result"];
        }
        if(result == true) {
            MOCK_PRINTF("Device return success:\n");
        } else {
            MOCK_PRINTF("Device return ERROR:\n");
        }
    } else {
        MOCK_PRINTF("Does not contain method: %s\n", data);
    }
    return false;
}

void JsonMockContainer::_clearAll(void)
{
    memset(this->_deviceUuidList, 0x00, sizeof(_deviceUuidList));
    memset(this->_valueUuidList, 0x00, sizeof(_valueUuidList));
    memset(this->_stateReportUuidList, 0x00, sizeof(_stateReportUuidList));
    memset(this->_stateControlUuidList, 0x00, sizeof(_stateControlUuidList));
    memset(this->_stateReportDataList, 0x00, sizeof(_stateReportDataList));
    memset(this->_stateControlDataList, 0x00, sizeof(_stateControlDataList));

    this->_numberDevices = 0;
    this->_numberValues = 0;
    this->_numberReportStates = 0;
    this->_numberControlStates = 0;

    this->_currentDevice = 0;
    this->_currentValue = 0;
    this->_currentReportState = 0;
    this->_currentControlState = 0;
}
