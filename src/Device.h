#pragma once

#include <Arduino.h>
#include "WappstoModel.h"
#include "WappstoIoT.h"
#include "WappstoRpc.h"
#include "Value.h"

#ifndef MAX_VALUES
    #define MAX_VALUES 10
#endif

typedef struct {
    String name;
    String product;
    String manufacturer;
    String description;
    String version;
    String serial;
    String protocol;
    String communication;
} DeviceDescription_t;

class Device;

typedef void (*WappstoDeviceCallback)(Device *device);

class Device: public WappstoModel
{
public:
    Device(WappstoModel *network, DeviceDescription_t *deviceInfo);
    Value* createValueNumber(ValueNumber_t *valNumber);
    Value* createValueNumber(ValueNumberFull_t *valNumber);
    Value* createValueString(ValueString_t *valString);
    Value* createValueBlob(ValueBlob_t *valBlob);
    Value* createValueXml(ValueXml_t *valXml);
    void post(void);

    String name;
    DeviceDescription_t deviceInfo;

    void onRefresh(WappstoDeviceCallback cb);
    void onDelete(WappstoDeviceCallback cb);

    void handlePeriod(void);

private:
    WappstoDeviceCallback _onRefreshCb;
    WappstoDeviceCallback _onDeleteCb;

    int currentNumberOfValues;
    Value* values[MAX_VALUES];

    Value** getFreeValue();
    Value* sendValue(Value* value);

    bool handleRefresh();
    bool handleDelete();
    bool handleUpdate(JsonObject obj);
    bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj);
    void getFindQuery(char *url);
    void toJSON(JsonObject data);
};
