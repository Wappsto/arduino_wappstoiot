#include "WappstoIoT.h"
#include "WappstoRpc.h"

#define WAPPSTO_PORT 443
const char* wappsto_server = "collector.wappsto.com";


Network::Network(WiFiClientSecure *client) : _wappstoRpc(client)
{
    _client = client;
}

bool Network::connect(String name, const char* network_id, const char* ca, const char* client_crt, const char* client_key)
{
    //strcpy(this->uuid, network_id);
    strcpy(this->uuid, network_id);
    this->name = name;

    currentNumberOfDevices = 0;
    _client->setCACert(ca);
    _client->setCertificate(client_crt);
    _client->setPrivateKey(client_key);

    this->_onChangeCb = NULL;
    this->_onRefreshCb = NULL;
    this->_onRequestCb = NULL;
    this->_onDeleteCb = NULL;

    if(_client->connect(wappsto_server, WAPPSTO_PORT)) {
        _wappstoRpc.postNetwork(this->uuid, this->name);
        // if have devices post them

        return true;
    }
    return false;
}

bool Network::disconnect(void)
{
    return false; //_client->disconnect();
}

void *_findObjectFromUuid(const String &uuid)
{
    /*
    for(int devs=0; devs<currentNumberOfDevices; devs++) {

        if(devices[devs] == NULL) {
            continue;
        }
        if(strcmp(devices[devs]->uuid, tmpUuid) == 0) {
            Serial.println("Found device requested UUID");
        }
        for(int vals=0; vals<devices[devs]->currentNumberOfValues; vals++) {
            if(devices[devs]->values[vals] == NULL) {
                continue;
            }
            if(strcmp(devices[devs]->values[vals]->uuid, tmpUuid) == 0) {
                Serial.println("Found value requested UUID");
            }
            if(strcmp(devices[devs]->values[vals]->reportState->uuid, tmpUuid) == 0) {
                Serial.println("Found state report requested UUID");
                if(req == REQUEST_GET) {
                    if(devices[devs]->values[vals]->_onRefreshCb) {
                        Serial.println("Calling callback refresh");
                        devices[devs]->values[vals]->_onRefreshCb(devices[devs]->values[vals], tmpData);
                        return true;
                    }
                }
            } else if(strcmp(devices[devs]->values[vals]->controlState->uuid, tmpUuid) == 0) {
                Serial.println("Found state control requested UUID");
                if(req == REQUEST_PUT) {
                    if(devices[devs]->values[vals]->_onControlCb) {
                        Serial.println("Calling callback control");
                        devices[devs]->values[vals]->_onControlCb(devices[devs]->values[vals], tmpData);
                        return true;
                    }
                }
            }
        }
    }
    */
    return NULL;
}

bool Network::dataAvailable(void)
{
    char tmpData[200];
    char tmpUuid[UUID_LENGTH];
    if(!_client->available()) {
        return false;
    }
    RequestType_e req = _wappstoRpc.readData(tmpUuid, tmpData);

    Serial.print(" type: ");
    Serial.println(req);

    if(tmpUuid && strlen(tmpUuid) > 0) {
        Serial.print("UUID: ");
        Serial.print(tmpUuid);
    } else {
        Serial.println("Invalid or no uuid");
        return false;
    }

    if(req == REQUEST_DELETE) {
        Serial.println("Delete not handled yet");
        return false;
    } else if(req == REQUEST_UNKNOWN) {
        Serial.println("Unknown request - not handled");
        return false;
    }

    for(int devs=0; devs<currentNumberOfDevices; devs++) {

        if(devices[devs] == NULL) {
            continue;
        }
        if(strcmp(devices[devs]->uuid, tmpUuid) == 0) {
            Serial.println("Found device requested UUID");
        }
        for(int vals=0; vals<devices[devs]->currentNumberOfValues; vals++) {
            if(devices[devs]->values[vals] == NULL) {
                continue;
            }
            if(strcmp(devices[devs]->values[vals]->uuid, tmpUuid) == 0) {
                Serial.println("Found value requested UUID");
                return true;
            }
            if(strcmp(devices[devs]->values[vals]->reportState->uuid, tmpUuid) == 0) {
                Serial.println("Found state report requested UUID");
                if(req == REQUEST_GET) {
                    if(devices[devs]->values[vals]->_onRefreshCb) {
                        devices[devs]->values[vals]->_onRefreshCb(devices[devs]->values[vals], tmpData);
                        return true;
                    }
                }
            } else if(strcmp(devices[devs]->values[vals]->controlState->uuid, tmpUuid) == 0) {
                Serial.println("Found state control requested UUID");
                if(req == REQUEST_PUT) {
                    if(devices[devs]->values[vals]->_onControlCb) {
                        devices[devs]->values[vals]->controlState->data = tmpData;
                        devices[devs]->values[vals]->_onControlCb(devices[devs]->values[vals], tmpData);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool Network::change(void)
{
    return false;
}

bool Network::deleteReq(void)
{
    return false;
}

Device* Network::createDevice(String name, String product, String manufacturer, String description, String protocol, String communication)
{
    if(currentNumberOfDevices >= MAX_DEVICES) {
        Serial.println("Cannot create more devices");
        return NULL;
    }
    currentNumberOfDevices++;
    devices[currentNumberOfDevices-1] = new Device(this, _wappstoRpc, currentNumberOfDevices-1, name, product, manufacturer, description, protocol, communication);
    devices[currentNumberOfDevices-1]->post();
    return devices[currentNumberOfDevices-1];
}

void Network::onChange(WappstoIoTCallback cb)
{
    this->_onChangeCb = cb;
}

void Network::onRefresh(WappstoIoTCallback cb)
{
    this->_onRefreshCb = cb;
}

void Network::onRequest(WappstoIoTCallback cb)
{
    this->_onRequestCb = cb;
}

void Network::onDelete(WappstoIoTCallback cb)
{
    this->_onDeleteCb = cb;
}
