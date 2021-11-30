#pragma once

#include <Arduino.h>
#include "WappstoIoT.h"
#include "WappstoRpc.h"

class Value
{
public:
    Value(Device *device, WappstoRpc &wappstoRpc, uint8_t id, String name, String type, PERMISSION_e permission, ValueNumber_t *valNumber);
    Value(Device *device, WappstoRpc &wappstoRpc, uint8_t id, String name, String type, PERMISSION_e permission, ValueString_t *valString);
    bool change(void);
    bool report(const String &data);
    bool control(String &data);
    bool request(String &data);
    bool deleteReq(void);
    void onControl(WappstoIoTCallback cb);
    void onChange(WappstoIoTCallback cb);
    void onRefresh(WappstoIoTCallback cb);
    void onRequest(WappstoIoTCallback cb);
    void onDelete(WappstoIoTCallback cb);
    void post(void);

    Device *parent;
    uint8_t id;
    char uuid[UUID_LENGTH];
    String name;
    String type;
    PERMISSION_e permission;
    VALUE_TYPE_e valueType;
    ValueNumber_t *valNumber;
    ValueString_t *valString;

    State* reportState;
    State* controlState;
    WappstoIoTCallback _onControlCb;
    WappstoIoTCallback _onChangeCb;
    WappstoIoTCallback _onRefreshCb;
    WappstoIoTCallback _onRequestCb;
    WappstoIoTCallback _onDeleteCb;

private:
    WappstoRpc &_wappstoRpc;
    void _init(void);
};
