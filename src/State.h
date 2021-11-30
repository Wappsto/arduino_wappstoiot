#pragma once

#include <Arduino.h>
#include "WappstoIoT.h"
#include "WappstoRpc.h"

class State
{
public:
    State(Value *value, WappstoRpc &wappstoRpc, StateType_e stateType);
    void control(String data);
    void report(String data);
    void deleteReq(void);

    void onChange(WappstoIoTCallback cb);
    void onRefresh(WappstoIoTCallback cb);
    void onRequest(WappstoIoTCallback cb);
    void onDelete(WappstoIoTCallback cb);

    Value *parent;
    StateType_e stateType;
    char uuid[UUID_LENGTH];
    String data;
    String timestamp;

private:
    WappstoRpc &_wappstoRpc;
    WappstoIoTCallback _onChangeCb;
    WappstoIoTCallback _onRefreshCb;
    WappstoIoTCallback _onRequestCb;
    WappstoIoTCallback _onDeleteCb;
};

