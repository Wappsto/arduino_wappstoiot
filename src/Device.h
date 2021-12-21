#pragma once

#include <Arduino.h>

#include "WappstoIoT.h"
#include "WappstoRpc.h"

class Device
{
public:
    Device(Network *network, DeviceDescription_t *deviceInfo);
    Value* createValueNumber(ValueNumber_t *valNumber);
    Value* createValueString(ValueString_t *valString);
    Value* createValueBlob(ValueBlob_t *valBlob);
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
    WappstoLog *_wappstoLog;
    WappstoCallback _onDeleteCb;
};
