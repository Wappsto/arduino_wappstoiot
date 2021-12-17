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

    bool change(void);
    bool deleteReq(void);
    Device* createDevice(String name, DeviceDescription_t *deviceInfo);

    void onChange(WappstoCallback cb);
    void onDelete(WappstoNetworkDeleteCallback cb);

    char uuid[UUID_LENGTH];
    String name;
    String description;
    int currentNumberOfDevices;
    Device* devices[MAX_DEVICES];

    WappstoNetworkDeleteCallback _onDeleteCb;

private:
    WappstoRpc *_wappstoRpc;

    WappstoCallback _onChangeCb;
};
