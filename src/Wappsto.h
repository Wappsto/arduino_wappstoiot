#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "WappstoIoT.h"
#include "WappstoRpc.h"

class Wappsto
{
public:
    Wappsto(WiFiClientSecure *client);
    Network* createNetwork(String name);
    Network* createNetwork(String name, String description);
    bool connect(const char* network_id, const char* ca, const char* client_crt, const char* client_key);
    bool disconnect(void);
    bool dataAvailable();

    char uuid[UUID_LENGTH];
    Network *network;


private:
    WappstoRpc _wappstoRpc;
    WiFiClientSecure *_client;
    Network *_network;
};
