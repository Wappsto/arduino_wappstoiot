#include "WappstoIoT.h"
#include "Wappsto.h"

#define WAPPSTO_PORT 443
#define WAPPSTO_SERVER "collector.wappsto.com"


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
    if(this->_client->connect(WAPPSTO_SERVER, WAPPSTO_PORT)) {
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
    this->_network->create();
    return this->_network;
}

Network *Wappsto::createNetwork(String name)
{
    this->_network = new Network(this->uuid, name, "");
    this->_network->create();
    return this->_network;
}

bool Wappsto::dataAvailable(void)
{
    UUID_t tmpUuid;

    if(!this->_client->connected()) {
        this->_wappstoLog->warning("Disconnected, trying reconnect");
        this->connect();
    }

    if(!this->_client->available()) {
        unsigned long currentMillis = millis();
        if(this->_pingIntervalMinutes != 0 && (currentMillis - this->_startPingMillis >= this->_pingIntervalMinutes*60*1000)) {
            this->_startPingMillis = currentMillis;
            this->_wappstoRpc->sendPing();
        }
        return false;
    }

    StaticJsonDocument<JSON_STATIC_BUFFER_SIZE> obj;
    RequestType_e req = this->_wappstoRpc->readData(tmpUuid, obj);

    if(req == REQUEST_UNKNOWN) {
        this->_wappstoLog->warning("Unknown request - not handled");
        return false;
    }

    if(tmpUuid && strlen(tmpUuid) > 0) {
        //Serial.print("UUID: ");
        //Serial.print(tmpUuid);
    } else {
        this->_wappstoLog->warning("Invalid or no uuid");
        return false;
    }

    return this->_network->handleRequest(tmpUuid, req, obj.as<JsonObject>());
}
