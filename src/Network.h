#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "WappstoIoT.h"
#include "WappstoRpc.h"


class Network
{
public:
    Network(const char* uuid, String name, String description);

    void post(void);
    Device* createDevice(DeviceDescription_t *deviceInfo);
    void onDelete(WappstoNetworkDeleteCallback cb);
    WappstoNetworkDeleteCallback _onDeleteCb;

    char uuid[UUID_LENGTH];
    String name;
    String description;
    int currentNumberOfDevices;
    Device* devices[MAX_DEVICES];

private:
    WappstoRpc *_wappstoRpc;
    WappstoLog *_wappstoLog;
};
