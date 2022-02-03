#pragma once

#include <Arduino.h>
#include "WappstoModel.h"
#include "WappstoIoT.h"

typedef enum {
    TYPE_REPORT,
    TYPE_CONTROL,
} StateType_e;

class State;

typedef void (*WappstoStateCallback)(State *state);

class State: public WappstoModel
{
public:
    State(WappstoModel *parent, StateType_e stateType, bool forceCreate);

    StateType_e stateType;
    String data;
    Timestamp_t timestamp;

private:
    void toJSON(JsonObject data);
    bool handleRefresh();
    bool handleDelete();
    bool handleUpdate(JsonObject doc);
    bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject doc);
    void getFindQuery(char *url);

    const char* typeToString();
};
