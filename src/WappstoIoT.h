#pragma once
#include <Arduino.h>
#include <WiFiClientSecure.h>

#define MAX_DEVICES 5
#define MAX_VALUES 10

#define UUID_LENGTH 37

void generateNewUuid(char* str);
const char* getUtcTime(void);

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

typedef struct {
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
    float min;
    float max;
    float step;
    String unit;
    String si_conversion;
} ValueNumber_t;

typedef struct
{
    int max;
    String encoding;
} ValueString_t;

typedef struct
{
    int max;
    String encoding;
} ValueBlob_t;

typedef enum {
    READ,
    WRITE,
    READ_WRITE,
} PERMISSION_e;

class Network;
class Device;
class Value;
class State;

typedef void (*WappstoCallback)(void *object);

typedef void (*WappstoNetworkDeleteCallback)(Network *network);
typedef void (*WappstoValueRefreshCallback)(Value *value);
typedef void (*WappstoValueControlStringCallback)(Value *value, String data, String timestamp);
typedef void (*WappstoValueControlNumberCallback)(Value *value, double data, String timestamp);

#include "Wappsto.h"
#include "Network.h"
#include "Device.h"
#include "Value.h"
#include "State.h"
