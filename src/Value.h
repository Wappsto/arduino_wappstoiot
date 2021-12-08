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
    bool control(const String &data);
    bool request(String &data);
    bool deleteReq(void);
    void onControl(WappstoValueControlCallback cb);
    void onChange(WappstoCallback cb);
    void onRefresh(WappstoValueRefreshCallback cb);
    void onDelete(WappstoCallback cb);
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
    WappstoValueControlCallback _onControlCb;
    WappstoCallback _onChangeCb;
    WappstoValueRefreshCallback _onRefreshCb;
    WappstoCallback _onDeleteCb;

    String getControlData(void);
    String getControlTimestamp(void);
    String getReportData(void);
    String getReportTimestamp(void);

private:
    WappstoRpc &_wappstoRpc;
    void _init(void);
};
