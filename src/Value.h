#pragma once

#include <Arduino.h>
#include "WappstoIoT.h"
#include "WappstoRpc.h"
#include "State.h"

typedef enum {
    READ,
    WRITE,
    READ_WRITE,
} PERMISSION_e;

typedef enum {
    NUMBER_VALUE,
    STRING_VALUE,
    BLOB_VALUE,
    XML_VALUE,
} VALUE_TYPE_e;

typedef struct
{
    String name;
    String type;
    PERMISSION_e permission;
    double min;
    double max;
    double step;
    String unit;
    String si_conversion;
} ValueNumber_t;

typedef struct
{
    String name;
    String type;
    PERMISSION_e permission;
    int max;
    String encoding;
} ValueString_t;

typedef struct
{
    String name;
    String type;
    PERMISSION_e permission;
    int max;
    String encoding;
} ValueBlob_t;

typedef struct
{
    String name;
    String type;
    PERMISSION_e permission;
    String xml_namespace;
    String xsd;
} ValueXml_t;

class Value;

typedef void (*WappstoValueCallback)(Value *value);
typedef void (*WappstoValueControlStringCallback)(Value *value, String data, String timestamp);
typedef void (*WappstoValueControlNumberCallback)(Value *value, double data, String timestamp);

class Value: public WappstoModel
{
public:
    Value(WappstoModel *device, ValueNumber_t *valNumber);
    Value(WappstoModel *device, ValueString_t *valString);
    Value(WappstoModel *device, ValueBlob_t *valBlob);
    Value(WappstoModel *device, ValueXml_t *valXml);
    bool report(const String &data);
    bool report(double data);
    bool report(int data);
    bool control(const String &data);
    bool control(double data);
    bool control(int data);
    bool request(String &data);
    void onControl(WappstoValueControlStringCallback cb);
    void onControl(WappstoValueControlNumberCallback cb);

    void createStates(void);
    bool handleStateCb(const char* tmpUuid, RequestType_e req, const char *tmpData, const char *tmpTimestamp);

    String name;
    String type;
    PERMISSION_e permission;
    VALUE_TYPE_e valueType;
    ValueNumber_t *valNumber;
    ValueString_t *valString;
    ValueBlob_t *valBlob;
    ValueXml_t *valXml;
    bool valueCreated;

    String getControlData(void);
    double getControlNumberData(void);
    String getControlTimestamp(void);
    String getReportData(void);
    double getReportNumberData(void);
    String getReportTimestamp(void);

    void onRefresh(WappstoValueCallback cb);
    void onDelete(WappstoValueCallback cb);

private:
    WappstoValueCallback _onRefreshCb;
    WappstoValueCallback _onDeleteCb;
    State* reportState;
    State* controlState;
    WappstoValueControlStringCallback _onControlStringCb;
    WappstoValueControlNumberCallback _onControlNumberCb;

    void _init(void);

    void toJSON(JsonObject data);
    bool handleRefresh();
    bool handleDelete();
    bool handleUpdate(JsonObject obj);
    bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj);
    void getFindQuery(char *url);
};
