#include "WappstoIoT.h"


State::State(Value *value, WappstoRpc &wappstoRpc, StateType_e stateType, bool forceCreate) : _wappstoRpc(wappstoRpc)
{
    this->parent = value;
    this->stateType = stateType;
    this->requiresPost = false;

    uint32_t readId = 0xFFFFFFFF;
    if(stateType == TYPE_REPORT) {
        readId  = (uint32_t)(((uint32_t)0x00 << 24) |
                                 ((uint32_t)parent->parent->id << 16) |
                                 ((uint32_t)parent->id << 8) |
                                  ((uint32_t)0x01));
    } else {
        readId  = (uint32_t)(((uint32_t)0x00 << 24) |
                             ((uint32_t)parent->parent->id << 16) |
                             ((uint32_t)parent->id << 8) |
                                  ((uint32_t)0x02));

    }

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
