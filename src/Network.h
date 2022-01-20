#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "WappstoModel.h"
#include "WappstoIoT.h"
#include "WappstoRpc.h"
#include "Device.h"

#ifndef MAX_DEVICES
    #define MAX_DEVICES 5
#endif

class Network: public WappstoModel
{
public:
    Network(const char* uuid, String name, String description);

    void post(void);
    Device* createDevice(DeviceDescription_t *deviceInfo);

    String name;
    String description;
    int currentNumberOfDevices;
    Device* devices[MAX_DEVICES];

private:
    void toJSON(JsonObject data);
    bool handleUpdate(JsonObject doc);
    bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject doc);
    void getFindQuery(char *url);
};
