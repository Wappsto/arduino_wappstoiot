#include "WappstoIoT.h"


Value::Value(Device *device, WappstoRpc &wappstoRpc, uint8_t id, String name, String type, PERMISSION_e permission, ValueNumber_t *valNumber) :
    parent(device), _wappstoRpc(wappstoRpc), id(id), name(name), permission(permission), valueType(valueType)
{
    _init();
    this->valueType = NUMBER_VALUE;
    this->valNumber = valNumber;
    this->valString = NULL;
}

Value::Value(Device *device, WappstoRpc &wappstoRpc, uint8_t id, String name, String type, PERMISSION_e permission, ValueString_t *valString) :
    parent(device), _wappstoRpc(wappstoRpc), id(id), name(name), permission(permission), valueType(valueType)
{
    _init();
    this->valueType = STRING_VALUE;
    this->valNumber = NULL;
    this->valString = valString;
}

void Value::_init(void)
{
    this->_onControlCb = NULL;;
    this->_onChangeCb = NULL;;
    this->_onRefreshCb = NULL;;
    this->_onDeleteCb = NULL;;

    uint32_t readId  = (uint32_t) (((uint32_t)0x00 << 24) |
                                   ((uint32_t)parent->id << 16) |
                                   ((uint32_t)id << 8) |
                                   ((uint32_t)0xFF));

    if(readUuid(readId) != NULL) {
        strcpy(this->uuid, readUuid(readId));
    } else {
        generateNewUuid(this->uuid);
        writeUuid(readId, this->uuid);
    }

    switch(permission) {
        case READ:
            reportState = new State(this, _wappstoRpc, TYPE_REPORT);
            break;
        case WRITE:
            controlState = new State(this, _wappstoRpc, TYPE_CONTROL);
            break;
        case READ_WRITE:
            reportState = new State(this, _wappstoRpc, TYPE_REPORT);
            controlState = new State(this, _wappstoRpc, TYPE_CONTROL);
            break;
    }

}

void Value::post(void)
{
    this->_wappstoRpc.postValue(this);
}

bool Value::change(void)
{
    return false;
}

bool Value::report(const String &data)
{
    this->reportState->timestamp = getUtcTime();
    this->reportState->data = data;
    _wappstoRpc.putState(this->reportState);
    return true;
}

bool Value::control(const String &data)
{
    this->controlState->timestamp = getUtcTime();
    this->controlState->data = data;
    _wappstoRpc.putState(this->controlState);
    return true;
}

bool Value::deleteReq(void)
{
    return false;
}

void Value::onControl(WappstoValueControlCallback cb)
{
    this->_onControlCb = cb;
}

void Value::onChange(WappstoCallback cb)
{
    this->_onChangeCb = cb;
}

void Value::onRefresh(WappstoValueRefreshCallback cb)
{
    this->_onRefreshCb = cb;
}

void Value::onDelete(WappstoCallback cb)
{
    this->_onDeleteCb = cb;
}
