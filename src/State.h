#pragma once

#include <Arduino.h>
#include "WappstoIoT.h"
#include "WappstoRpc.h"

class State
{
public:
    State(Value *value, WappstoRpc &wappstoRpc, StateType_e stateType, bool forceCreate);
    void control(String data);
    void report(String data);
    void deleteReq(void);

    void onChange(WappstoCallback cb);
    void onDelete(WappstoCallback cb);

    Value *parent;
    StateType_e stateType;
    char uuid[UUID_LENGTH];
    String data;
    String timestamp;
    bool requiresPost;

private:
    WappstoRpc &_wappstoRpc;
    WappstoCallback _onChangeCb;
    WappstoCallback _onDeleteCb;
};

