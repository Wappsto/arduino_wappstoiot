#include "WappstoIoT.h"


State::State(Value *value, StateType_e stateType, bool forceCreate)
{
    this->_wappstoRpc = WappstoRpc::instance();
    this->parent = value;
    this->stateType = stateType;
    this->requiresPost = false;
    this->data = "";
    memset(this->timestamp, 0, TIMESTAMP_LENGTH);

    if(forceCreate) {
        generateNewUuid(this->uuid);
        this->requiresPost = true;
    } else if(_wappstoRpc->getStateUuidFromName(value, stateType, this->uuid)) {
        this->_wappstoRpc->getStateDataTime(this->uuid, this->data, &this->timestamp);
    } else {
        generateNewUuid(this->uuid);
        this->requiresPost = true;
    }
}
