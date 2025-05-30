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

class WNetwork;

//typedef void (*WappstoNetworkCallback)(WNetwork *WNetwork);
typedef void (*WappstoNetworkCallback)(WNetwork *network);

class WNetwork: public WappstoModel
{
public:
    WNetwork(const char* uuid, String name, String description);

    void post(void);
    Device* createDevice(DeviceDescription_t *deviceInfo);

    void onRefresh(WappstoNetworkCallback cb);
    void onDelete(WappstoNetworkCallback cb);
    void handlePeriod(void);

    String name;
    String description;
    int currentNumberOfDevices;
    Device* devices[MAX_DEVICES];

private:
    WappstoNetworkCallback _onRefreshCb;
    WappstoNetworkCallback _onDeleteCb;
    bool handleRefresh();
    bool handleDelete();
    bool handleUpdate(JsonObject doc);
    bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject doc);
    void getFindQuery(char *url);
    void toJSON(JsonObject data);
};
