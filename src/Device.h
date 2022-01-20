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

class Device: public WappstoModel
{
public:
    Device(WappstoModel *network, DeviceDescription_t *deviceInfo);
    Value* createValueNumber(ValueNumber_t *valNumber);
    Value* createValueString(ValueString_t *valString);
    Value* createValueBlob(ValueBlob_t *valBlob);
    Value* createValueXml(ValueXml_t *valXml);
    void post(void);

    String name;
    DeviceDescription_t deviceInfo;

private:
    int currentNumberOfValues;
    Value* values[MAX_VALUES];

    Value** getFreeValue();
    Value* sendValue(Value* value);

    void toJSON(JsonObject data);
    bool handleUpdate(JsonObject obj);
    bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj);
    void getFindQuery(char *url);
};
