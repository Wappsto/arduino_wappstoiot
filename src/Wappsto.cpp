#include "WappstoIoT.h"

#define WAPPSTO_PORT 443
const char* wappsto_server = "collector.wappsto.com";


Wappsto::Wappsto(WiFiClientSecure *client)
{
    this->_client = client;
    this->_network = NULL;
    this->_wappstoLog = WappstoLog::instance();
    this->_wappstoRpc = WappstoRpc::instance();
    this->_wappstoRpc->init(this->_client);
}

void Wappsto::config(const char* network_id, const char* ca, const char* client_crt, const char* client_key, int pingInterval, LOG_LEVELS_e logLevel)
{
    strcpy(this->uuid, network_id);

    this->_client->setCACert(ca);
    this->_client->setCertificate(client_crt);
    this->_client->setPrivateKey(client_key);
    this->_pingIntervalMinutes = pingInterval;
    this->_startPingMillis = millis();
    this->_wappstoLog->setLogLevel(logLevel);
}

void Wappsto::config(const char* network_id, const char* ca, const char* client_crt, const char* client_key)
{
    this->config(network_id, ca, client_crt, client_key, 0, NO_LOGS);
}

bool Wappsto::connect(void)
{
    if(this->_client->connect(wappsto_server, WAPPSTO_PORT)) {
        return true;
    }
    this->_wappstoLog->error("Could not connect to wappsto");
    return false;
}

bool Wappsto::disconnect(void)
{
    if(this->_client->connected()) {
        this->_client->stop();
        return true;
    }
    return false;
}

Network *Wappsto::createNetwork(String name, String description)
{
    this->_network = new Network(this->uuid, name, description);
    this->_network->post();
    return this->_network;
}

Network *Wappsto::createNetwork(String name)
{
    this->_network = new Network(this->uuid, name, "");
    this->_network->post();
    return this->_network;
}

bool Wappsto::dataAvailable(void)
{
    char tmpData[1200] = {0,};
    char tmpTimestamp[28] = {0,};
    UUID_t tmpUuid;

    if(!this->_client->connected()) {
        this->_wappstoLog->warning("Disconnected, trying reconnect");
        this->connect();
    }

    if(!this->_client->available()) {
        unsigned long currentMillis = millis();
        if(_pingIntervalMinutes != 0 && (currentMillis - _startPingMillis >= _pingIntervalMinutes*60*1000)) {
            _startPingMillis = currentMillis;
            this->_wappstoRpc->sendPing();
        }
        return false;
    }
    RequestType_e req = this->_wappstoRpc->readData(tmpUuid, tmpData, tmpTimestamp);

    if(tmpUuid && strlen(tmpUuid) > 0) {
        //Serial.print("UUID: ");
        //Serial.print(tmpUuid);
    } else {
        this->_wappstoLog->warning("Invalid or no uuid");
        return false;
    }

    if(req == REQUEST_DELETE) {
        if(strcmp(this->uuid, tmpUuid) == 0 && this->_network->_onDeleteCb) {
            this->_network->_onDeleteCb(this->_network);
            return true;
        }
        this->_wappstoLog->warning("Delete not handled yet");
        return false;
    } else if(req == REQUEST_UNKNOWN) {
        this->_wappstoLog->warning("Unknown request - not handled");
        return false;
    }

    for(int devs=0; devs < this->_network->currentNumberOfDevices; devs++) {

        if(this->_network->devices[devs] == NULL) {
            continue;
        }
        if(strcmp(this->_network->devices[devs]->uuid, tmpUuid) == 0) {
            this->_wappstoLog->warning("Found device requested UUID - not handled");
        }
        for(int vals=0; vals < this->_network->devices[devs]->currentNumberOfValues; vals++) {
            if(this->_network->devices[devs]->values[vals] == NULL) {
                continue;
            }
            if(strcmp(this->_network->devices[devs]->values[vals]->uuid, tmpUuid) == 0) {
                return true;
            }
            if(this->_network->devices[devs]->values[vals]->handleStateCb(tmpUuid, req, tmpData, tmpTimestamp)) {
                return true;
            }
        }
    }
    return false;
}
