#pragma once
#include <Arduino.h>
#include <WiFiClientSecure.h>

#define MAX_DEVICES 5
#define MAX_VALUES 10

#define UUID_LENGTH 37
typedef void (*WappstoIoTCallback)(void *object, String data);

void generateNewUuid(char* str);
const char* getUtcTime(void);

const char* readUuid(uint32_t storeId);
bool writeUuid(uint32_t storeId, const char* uuid);

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
} DeviceInfo_t;

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

class Device;
class Value;
class State;


#include "Network.h"
#include "Device.h"
#include "Value.h"
#include "State.h"






#if 0
**When a Unit receives:**

| Method | Service |                     Meanings                   |         IoT-API          |
|  ----  |  ----   |                       ----                     |           ---            |
| POST   | NETWORK | This behavior is not possible.                 |                          |
| GET    | NETWORK | No defined behavior.                           | WappstoIoT.onChange      |
|        |         |                                                | WappstoIoT.onRefresh     |
| PUT    | NETWORK | Change the Network Name/Description            | WappstoIoT.onRequest     |
| DELETE | NETWORK | Used to soft reset the unit, with a following  | WappstoIoT.onChange      |
|        |         | recreation of the entire model.                | WappstoIoT.onDelete      |
|        |
| POST   | DEVICE  | Request a creation of Device. Can be refused.  | WappstoIoT.onRequest     |
|        |         |                                                | device.onRequest         |
|        |         |                                                | value.onRequest          |
| GET    | DEVICE  | No defined behavior.                           | device.onRequest         |
|        |         |                                                | device.onRefresh         |
| PUT    | DEVICE  | Change a parameter (like Version/Manufacture)  | device.onChange          |
| DELETE | DEVICE  | Used to remove a Device. (Informational)       | device.onRequest         |
|        |         |                                                | device.onDelete          |
|        |
| POST   | VALUE   | Request a creation of Value. Can be refused.   | device.onRequest         |
|        |         |                                                | value.onChnage           |
| GET    | VALUE   | Used to trigger a report state refresh.        | value.onRequest          |
|        |         |                                                | value.onRefresh          |
| PUT    | VALUE   | Change a parameter (like Delta/period)         | value.onChange           |
| DELETE | VALUE   | Used to remove a Value. (Informational)        | value.onRequest          |
|        |         |                                                | value.onDelete           |
|        |
| POST   | STATE   | Request a creation of State. Can be refused.   | value.onRequest          |
|        |         |                                                | state.onRequest          |
| GET    | STATE   | Used to trigger a refresh                      | value.onRequest          |
|        |         |                                                | value.onRefresh          |
|        |         |                                                | state.onRequest          |
|        |         |                                                | state.onRefresh
| PUT    | STATE   | (Control) Used to change a state               | (control)value.onRequest |
|        |         |                                                | (control)value.onControl |
|        |         |                                                | state.onRequest
| PUT    | STATE   | (Report) Error, can not control a report.      |                          |
| DELETE | STATE   | Used to remove a State. (Informational)        | value.onRequest          |
|        |         |                                                | state.onDelete
#endif


#if 0
**When a Unit sends:**

|         IoT-API          |                     Meanings                   | Method | Service |
|           ---            |                       ----                     |  ----  |  ----   |
| WappstoIoT.connect       | Request a creation of Network. Can be refused. | POST   | NETWORK |
| (WappstoIoT.config)      |                                                | GET    | NETWORK |
| WappstoIoT.change        |                                                | PUT    | NETWORK |
| WappstoIoT.delete        | Used to unclaim a Network. Can be refused.     | DELETE | NETWORK |
|                          |
| WappstoIoT.createDevice  | Request a creation of Device. Can be refused.  | POST   | DEVICE  |
| (WappstoIoT.config)      |                                                | GET    | DEVICE  |
| device.change            |                                                | PUT    | DEVICE  |
| device.delete            | Used to remove a Device. (Informational)       | DELETE | DEVICE  |
|                          |
| device.createValue       | Request a creation of Value. Can be refused.   | POST   | VALUE   |
| (WappstoIoT.config)      |                                                | GET    | VALUE   |
| value.change             |                                                | PUT    | VALUE   |
| value.delete             | Used to remove a Value. (Informational)        | DELETE | VALUE   |
|                          |
| device.createValue       | Request a creation of State. Can be refused.   | POST   | STATE   |
| (WappstoIoT.config)      |                                                | GET    | STATE   |
| value.report  (Report)   |                                                | PUT    | STATE   |
| value.change  (Report)   |                                                |        |         |
| state.control (Control)  |                                                |        |         |
| value.request (Control)  |                                                |        |         |
| value.change (Permission)| Used to remove a State. (Informational)        | DELETE | STATE   |
| state.delete             |                                                |        |         |
#endif
