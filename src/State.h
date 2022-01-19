#pragma once

#include <Arduino.h>
#include "WappstoIoT.h"
#include "WappstoRpc.h"

class State
{
public:
    State(Value *value, StateType_e stateType, bool forceCreate);

    Value *parent;
    StateType_e stateType;
    UUID_t uuid;
    String data;
    Timestamp_t timestamp;
    bool requiresPost;

private:
    WappstoRpc *_wappstoRpc;
};
