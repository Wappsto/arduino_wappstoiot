#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "WappstoIoT.h"
#include "WappstoRpc.h"
#include "WappstoModel.h"
#include "WNetwork.h"
#include "Device.h"
#include "Value.h"
#include "State.h"

class Wappsto
{
public:
    Wappsto(WiFiClientSecure *client);
    WNetwork* createNetwork(String name);
    WNetwork* createNetwork(String name, String description);
    int config(const char* network_id, const char* ca, const char* client_crt, const char* client_key);
    int config(const char* network_id, const char* ca, const char* client_crt, const char* client_key, int pingInterval, LOG_LEVELS_e logLevel);
    bool connect(void);
    bool disconnect(void);
    bool dataAvailable();


private:
    UUID_t uuid;
    WappstoRpc *_wappstoRpc;
    WappstoLog *_wappstoLog;
    WiFiClientSecure *_client;
    WNetwork *_network;
    int _pingIntervalMinutes;
    unsigned long _startPingMillis;
};
