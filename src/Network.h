#pragma once

#include <Arduino.h>
#include <vector>
#include <WiFiClientSecure.h>

#include "WappstoIoT.h"
#include "WappstoRpc.h"

class Network
{
public:
    Network(WiFiClientSecure *client);
    bool connect(String name, const char* network_id, const char* ca, const char* client_crt, const char* client_key);
    bool disconnect(void);
    bool dataAvailable();

    bool change(void);
    bool deleteReq(void);
    Device* createDevice(String name, String product, String manufacturer, String description, String protocol, String communication);

    void onChange(WappstoIoTCallback cb);
    void onRefresh(WappstoIoTCallback cb);
    void onRequest(WappstoIoTCallback cb);
    void onDelete(WappstoIoTCallback cb);

    char uuid[UUID_LENGTH];
    String name;
    int currentNumberOfDevices;
    Device* devices[MAX_DEVICES];
    void *_findObjectFromUuid(const String &uuid);


private:
    WappstoRpc _wappstoRpc;
    WiFiClientSecure *_client;

    WappstoIoTCallback _onChangeCb;
    WappstoIoTCallback _onRefreshCb;
    WappstoIoTCallback _onRequestCb;
    WappstoIoTCallback _onDeleteCb;
};
