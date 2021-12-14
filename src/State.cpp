#include "WappstoIoT.h"


State::State(Value *value, WappstoRpc &wappstoRpc, StateType_e stateType, bool forceCreate) : _wappstoRpc(wappstoRpc)
{
    this->parent = value;
    this->stateType = stateType;
    this->requiresPost = false;

    if(forceCreate) {
        generateNewUuid(this->uuid);
        this->requiresPost = true;
    } else if(!_wappstoRpc.getStateUuidFromName(value, stateType, this->uuid)) {
        generateNewUuid(this->uuid);
        this->requiresPost = true;
    }
    this->_onChangeCb = NULL;
    this->_onDeleteCb = NULL;
}

void State::control(String data)
{

}

void State::report(String data)
{

}

void State::deleteReq(void)
{

}
