#pragma once

#include <Arduino.h>

#include "WappstoIoT.h"
#include "WappstoRpc.h"

class Device
{
public:
    Device(Network *network, String name, DeviceDescription_t *deviceInfo);
    bool change(void);
    bool deleteReq(void);
    Value* createValueNumber(String name, String type, PERMISSION_e permission, ValueNumber_t *valNumber);
    Value* createValueString(String name, String type, PERMISSION_e permission, ValueString_t *valString);
    Value* createValueBlob(String name, String type, PERMISSION_e permission, ValueBlob_t *valBlob);
    void onChange(WappstoCallback cb);
    void onDelete(WappstoCallback cb);
    void post(void);

    Network *parent;
    char uuid[UUID_LENGTH];
    String name;
    DeviceDescription_t deviceInfo;

    int currentNumberOfValues;
    Value* values[MAX_VALUES];

private:
    WappstoRpc *_wappstoRpc;
    WappstoCallback _onChangeCb;
    WappstoCallback _onDeleteCb;
};
