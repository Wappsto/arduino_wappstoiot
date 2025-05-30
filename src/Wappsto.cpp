#include "WappstoIoT.h"
#include "Wappsto.h"

#define WAPPSTO_PORT 443
#define WAPPSTO_SERVER "collector.wappsto.com"


Wappsto::Wappsto(WiFiClientSecure *client)
{
    this->_wappstoLog = WappstoLog::instance();
    if(!client) {
        this->_wappstoLog->error("Client is a NULL pointer");
        return;
    }

    memset(this->uuid, 0, UUID_LENGTH);
    this->_client = client;
    this->_network = NULL;

    this->_wappstoRpc = WappstoRpc::instance();
    this->_wappstoRpc->init(this->_client);
}

int Wappsto::config(const char* network_id, const char* ca, const char* client_crt, const char* client_key, int pingInterval, LOG_LEVELS_e logLevel)
{
    if(!network_id || strlen(network_id) != 36) {
        this->_wappstoLog->error("Network ID is invalid. It should be an UUID");
        return -1;
    }
    strcpy(this->uuid, network_id);

    this->_client->setCACert(ca);
    this->_client->setCertificate(client_crt);
    this->_client->setPrivateKey(client_key);
    this->_pingIntervalMinutes = pingInterval;
    this->_startPingMillis = millis();
    this->_wappstoLog->setLogLevel(logLevel);

    return 0;
}

int Wappsto::config(const char* network_id, const char* ca, const char* client_crt, const char* client_key)
{
    return this->config(network_id, ca, client_crt, client_key, 0, NO_LOGS);
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

WNetwork *Wappsto::createNetwork(String name, String description)
{
    this->_network = new WNetwork(this->uuid, name, description);
    if(!this->_network->create()) {
        this->_wappstoLog->error("Could not create Network");
        this->_network = NULL;
    }
    return this->_network;
}

WNetwork *Wappsto::createNetwork(String name)
{
    return this->createNetwork(name, "");
}

bool Wappsto::dataAvailable(void)
{
    UUID_t tmpUuid;

    if(!this->_client->connected()) {
        this->_wappstoLog->warning("Disconnected, trying reconnect");
        this->connect();
    }

    if(!this->_client->available() && !this->_wappstoRpc->rpcAvailable()) {
        this->_network->handlePeriod();

        unsigned long currentMillis = millis();
        if(this->_pingIntervalMinutes != 0 && (currentMillis - this->_startPingMillis >= (unsigned long)this->_pingIntervalMinutes*60*1000)) {
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
