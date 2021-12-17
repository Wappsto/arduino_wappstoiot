#pragma once

#include <Arduino.h>
#include "WappstoIoT.h"
#include "WappstoRpc.h"

class Value
{
public:
    Value(Device *device, String name, String type, PERMISSION_e permission, ValueNumber_t *valNumber);
    Value(Device *device, String name, String type, PERMISSION_e permission, ValueString_t *valString);
    Value(Device *device, String name, String type, PERMISSION_e permission, ValueBlob_t *valBlob);
    bool change(void);
    bool report(const String &data);
    bool report(double data);
    bool report(int data);
    bool control(const String &data);
    bool control(double data);
    bool control(int data);
    bool request(String &data);
    bool deleteReq(void);
    void onControl(WappstoValueControlStringCallback cb);
    void onControl(WappstoValueControlNumberCallback cb);
    void onChange(WappstoCallback cb);
    void onRefresh(WappstoValueRefreshCallback cb);
    void onDelete(WappstoCallback cb);
    void post(void);

    Device *parent;
    char uuid[UUID_LENGTH];
    String name;
    String type;
    PERMISSION_e permission;
    VALUE_TYPE_e valueType;
    ValueNumber_t *valNumber;
    ValueString_t *valString;
    ValueBlob_t *valBlob;
    bool valueCreated;

    State* reportState;
    State* controlState;
    WappstoValueControlStringCallback _onControlStringCb;
    WappstoValueControlNumberCallback _onControlNumberCb;
    WappstoCallback _onChangeCb;
    WappstoValueRefreshCallback _onRefreshCb;
    WappstoCallback _onDeleteCb;

    String getControlData(void);
    double getControlNumberData(void);
    String getControlTimestamp(void);
    String getReportData(void);
    double getReportNumberData(void);
    String getReportTimestamp(void);

private:
    WappstoRpc *_wappstoRpc;
    void _init(String type);
};
