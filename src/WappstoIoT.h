#pragma once
#include <Arduino.h>
#include <WiFiClientSecure.h>

#ifndef MAX_DEVICES
    #define MAX_DEVICES 5
#endif

#ifndef MAX_VALUES
    #define MAX_VALUES 10
#endif

#define UUID_LENGTH 37

void generateNewUuid(char* str);
const char* getUtcTime(void);

class Network;
class Device;
class Value;
class State;

typedef enum {
    TYPE_REPORT,
    TYPE_CONTROL,
} StateType_e;

typedef enum {
    REQUEST_GET,
    REQUEST_PUT,
    REQUEST_DELETE,
    REQUEST_SUCCESS,
    REQUEST_UNKNOWN,
} RequestType_e;

typedef enum {
    READ,
    WRITE,
    READ_WRITE,
} PERMISSION_e;

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

typedef enum {
    NUMBER_VALUE,
    STRING_VALUE,
    BLOB_VALUE,
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

typedef void (*WappstoCallback)(void *object);

typedef void (*WappstoNetworkDeleteCallback)(Network *network);
typedef void (*WappstoValueRefreshCallback)(Value *value);
typedef void (*WappstoValueControlStringCallback)(Value *value, String data, String timestamp);
typedef void (*WappstoValueControlNumberCallback)(Value *value, double data, String timestamp);

#include "WappstoLog.h"
#include "Wappsto.h"
#include "Network.h"
#include "Device.h"
#include "Value.h"
#include "State.h"
