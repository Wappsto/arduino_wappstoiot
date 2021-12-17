#include "WappstoIoT.h"


State::State(Value *value, StateType_e stateType, bool forceCreate)
{
    _wappstoRpc = WappstoRpc::instance();
    this->parent = value;
    this->stateType = stateType;
    this->requiresPost = false;
    this->data = "";
    this->timestamp = "";

    if(forceCreate) {
        generateNewUuid(this->uuid);
        this->requiresPost = true;
    } else if(_wappstoRpc->getStateUuidFromName(value, stateType, this->uuid)) {
        _wappstoRpc->getStateDataTime(this->uuid, this->data, this->timestamp);
    } else {
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
