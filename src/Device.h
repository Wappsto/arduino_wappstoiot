#pragma once

#include <Arduino.h>

#include "WappstoIoT.h"
#include "WappstoRpc.h"

class Device
{
public:
    Device(Network *network, WappstoRpc &wappstoRpc, uint8_t id, String name, String product, String manufacturer, String description, String protocol, String communication);
    bool change(void);
    bool deleteReq(void);
    Value* createValueNumber(String name, String type, PERMISSION_e permission, ValueNumber_t *valNumber);
    Value* createValueString(String name, String type, PERMISSION_e permission, ValueString_t *valString);
    void onChange(WappstoCallback cb);
    void onDelete(WappstoCallback cb);
    void post(void);

    Network *parent;
    uint8_t id;
    char uuid[UUID_LENGTH];
    String name;
    DeviceInfo_t deviceInfo;

    int currentNumberOfValues;
    Value* values[MAX_VALUES];

private:
    WappstoRpc &_wappstoRpc;
    WappstoCallback _onChangeCb;
    WappstoCallback _onDeleteCb;
};
