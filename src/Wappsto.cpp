#include "WappstoIoT.h"

#define WAPPSTO_PORT 443
const char* wappsto_server = "collector.wappsto.com";

Wappsto::Wappsto(WiFiClientSecure *client)
{
    _client = client;
    _network = NULL;
    _wappstoRpc = WappstoRpc::instance();
    _wappstoRpc->init(_client);
}

void Wappsto::config(const char* network_id, const char* ca, const char* client_crt, const char* client_key)
{
    strcpy(this->uuid, network_id);

    _client->setCACert(ca);
    _client->setCertificate(client_crt);
    _client->setPrivateKey(client_key);
    this->_pingIntervalMinutes = 0;
    this->_startPingMillis = millis();
}

void Wappsto::config(const char* network_id, const char* ca, const char* client_crt, const char* client_key, int pingInterval)
{
    strcpy(this->uuid, network_id);

    _client->setCACert(ca);
    _client->setCertificate(client_crt);
    _client->setPrivateKey(client_key);
    this->_pingIntervalMinutes = pingInterval;
    this->_startPingMillis = millis();
}

bool Wappsto::connect(void)
{
    if(_client->connect(wappsto_server, WAPPSTO_PORT)) {
        return true;
    }
    return false;
}

bool Wappsto::disconnect(void)
{
    if(_client->connected()) {
        _client->stop();
        return true;
    }
    return false;
}

void Wappsto::setLog(bool enabled)
{
    _wappstoRpc->setDebug(enabled);
}

Network *Wappsto::createNetwork(String name, String description)
{
    _network = new Network(this->uuid, name, description);
    _network->post();
    return _network;
}

Network *Wappsto::createNetwork(String name)
{
    _network = new Network(this->uuid, name, "");
    _network->post();
    return _network;
}

bool Wappsto::dataAvailable(void)
{
    char tmpData[1200] = {0,};
    char tmpTimestamp[28] = {0,};
    char tmpUuid[UUID_LENGTH];

    if(!_client->available()) {
        unsigned long currentMillis = millis();
        if(currentMillis - _startPingMillis >= _pingIntervalMinutes*60*1000) {
            _startPingMillis = currentMillis;
            _wappstoRpc->sendPing();
        }
        return false;
    }
    RequestType_e req = _wappstoRpc->readData(tmpUuid, tmpData, tmpTimestamp);

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
            Serial.println("Found device requested UUID - not handled");
        }
        for(int vals=0; vals < _network->devices[devs]->currentNumberOfValues; vals++) {
            if(_network->devices[devs]->values[vals] == NULL) {
                continue;
            }
            if(strcmp(_network->devices[devs]->values[vals]->uuid, tmpUuid) == 0) {
                return true;
            }
            if(_network->devices[devs]->values[vals]->reportState && strcmp(_network->devices[devs]->values[vals]->reportState->uuid, tmpUuid) == 0) {
                if(req == REQUEST_GET) {
                    if(_network->devices[devs]->values[vals]->_onRefreshCb) {
                        _network->devices[devs]->values[vals]->_onRefreshCb(_network->devices[devs]->values[vals]);
                        return true;
                    }
                }
            } else if(_network->devices[devs]->values[vals]->controlState && strcmp(_network->devices[devs]->values[vals]->controlState->uuid, tmpUuid) == 0) {
                if(req == REQUEST_PUT) {
                    if(_network->devices[devs]->values[vals]->_onControlNumberCb) {
                        _network->devices[devs]->values[vals]->controlState->data = tmpData;
                        _network->devices[devs]->values[vals]->controlState->timestamp = tmpTimestamp;
                        double tmpDouble = _network->devices[devs]->values[vals]->controlState->data.toDouble();
                        _network->devices[devs]->values[vals]->_onControlNumberCb(_network->devices[devs]->values[vals],
                                                                                    tmpDouble,
                                                                                    _network->devices[devs]->values[vals]->controlState->timestamp);
                        return true;
                    } else if(_network->devices[devs]->values[vals]->_onControlStringCb) {
                        _network->devices[devs]->values[vals]->controlState->data = tmpData;
                        _network->devices[devs]->values[vals]->controlState->timestamp = tmpTimestamp;
                        _network->devices[devs]->values[vals]->_onControlStringCb(_network->devices[devs]->values[vals],
                                                                                    _network->devices[devs]->values[vals]->controlState->data,
                                                                                    _network->devices[devs]->values[vals]->controlState->timestamp);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
