#pragma once

#include <Arduino.h>
#include "WappstoModel.h"
#include "WappstoIoT.h"

class State: public WappstoModel
{
public:
    State(WappstoModel *parent, StateType_e stateType, bool forceCreate);

    StateType_e stateType;
    String data;
    Timestamp_t timestamp;

private:
    void toJSON(JsonObject data);
    bool handleUpdate(JsonObject doc);
    bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject doc);
    void getFindQuery(char *url);

    const char* typeToString();
};
