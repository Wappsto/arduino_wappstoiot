#include "WappstoIoT.h"

#define WAPPSTO_PORT 443
const char* wappsto_server = "collector.wappsto.com";

Wappsto::Wappsto(WiFiClientSecure *client) : _wappstoRpc(client)
{
    _client = client;
    _network = NULL;
}

void Wappsto::config(const char* network_id, const char* ca, const char* client_crt, const char* client_key)
{
    strcpy(this->uuid, network_id);

    _client->setCACert(ca);
    _client->setCertificate(client_crt);
    _client->setPrivateKey(client_key);
}

bool Wappsto::connect(void)
{
    if(_client->connect(wappsto_server, WAPPSTO_PORT)) {
        return true;
    }
    return false;
}

Network *Wappsto::createNetwork(String name, String description)
{
    _network = new Network(_wappstoRpc, this->uuid, name, description);
    _network->post();
    return _network;
}

Network *Wappsto::createNetwork(String name)
{
    _network = new Network(_wappstoRpc, this->uuid, name, "");
    _network->post();
    return _network;
}

bool Wappsto::dataAvailable(void)
{
    char tmpData[200];
    char tmpUuid[UUID_LENGTH];
    if(!_client->available()) {
        return false;
    }
    RequestType_e req = _wappstoRpc.readData(tmpUuid, tmpData);

    if(tmpUuid && strlen(tmpUuid) > 0) {
        //Serial.print("UUID: ");
        //Serial.print(tmpUuid);
    } else {
        Serial.println("Invalid or no uuid");
        return false;
    }

    if(req == REQUEST_DELETE) {
        if(strcmp(this->uuid, tmpUuid) == 0 && this->_network->_onDeleteCb) {
            this->_network->_onDeleteCb(this->_network);
            return true;
        }
        Serial.println("Delete not handled yet");
        return false;
    } else if(req == REQUEST_UNKNOWN) {
        Serial.println("Unknown request - not handled");
        return false;
    }

    for(int devs=0; devs < _network->currentNumberOfDevices; devs++) {

        if(_network->devices[devs] == NULL) {
            continue;
        }
        if(strcmp(_network->devices[devs]->uuid, tmpUuid) == 0) {
            Serial.println("Found device requested UUID");
        }
        for(int vals=0; vals < _network->devices[devs]->currentNumberOfValues; vals++) {
            if(_network->devices[devs]->values[vals] == NULL) {
                continue;
            }
            if(strcmp(_network->devices[devs]->values[vals]->uuid, tmpUuid) == 0) {
                Serial.println("Found value requested UUID");
                return true;
            }
            if(_network->devices[devs]->values[vals]->reportState && strcmp(_network->devices[devs]->values[vals]->reportState->uuid, tmpUuid) == 0) {
                Serial.println("Found state report requested UUID");
                if(req == REQUEST_GET) {
                    if(_network->devices[devs]->values[vals]->_onRefreshCb) {
                        _network->devices[devs]->values[vals]->_onRefreshCb(_network->devices[devs]->values[vals]);
                        return true;
                    }
                }
            } else if(_network->devices[devs]->values[vals]->controlState && strcmp(_network->devices[devs]->values[vals]->controlState->uuid, tmpUuid) == 0) {
                Serial.println("Found state control requested UUID");
                if(req == REQUEST_PUT) {
                    if(_network->devices[devs]->values[vals]->_onControlNumberCb) {
                        _network->devices[devs]->values[vals]->controlState->data = tmpData;
                        double tmpDouble = _network->devices[devs]->values[vals]->controlState->data.toDouble();
                        _network->devices[devs]->values[vals]->_onControlNumberCb(_network->devices[devs]->values[vals], tmpDouble, "");
                        return true;
                    } else if(_network->devices[devs]->values[vals]->_onControlStringCb) {
                        _network->devices[devs]->values[vals]->controlState->data = tmpData;
                        _network->devices[devs]->values[vals]->_onControlStringCb(_network->devices[devs]->values[vals], tmpData, "");
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
