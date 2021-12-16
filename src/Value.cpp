#include "WappstoIoT.h"


Value::Value(Device *device, WappstoRpc &wappstoRpc, String name, String type, PERMISSION_e permission, ValueNumber_t *valNumber) :
    parent(device), _wappstoRpc(wappstoRpc), name(name), permission(permission), valueType(valueType)
{
    _init(type);
    this->valueType = NUMBER_VALUE;
    this->valNumber = valNumber;
    this->valString = NULL;
}

Value::Value(Device *device, WappstoRpc &wappstoRpc, String name, String type, PERMISSION_e permission, ValueString_t *valString) :
    parent(device), _wappstoRpc(wappstoRpc), name(name), permission(permission), valueType(valueType)
{
    _init(type);
    this->valueType = STRING_VALUE;
    this->valNumber = NULL;
    this->valString = valString;
}

Value::Value(Device *device, WappstoRpc &wappstoRpc, String name, String type, PERMISSION_e permission, ValueBlob_t *valBlob) :
    parent(device), _wappstoRpc(wappstoRpc), name(name), permission(permission), valueType(valueType)
{
    _init(type);
    this->valueType = BLOB_VALUE;
    this->valNumber = NULL;
    this->valString = NULL;
    this->valBlob = valBlob;
}

void Value::_init(String type)
{
    this->valueCreated = false;
    this->type = type;
    this->reportState = NULL;
    this->controlState = NULL;
    this->_onControlStringCb = NULL;
    this->_onControlNumberCb = NULL;
    this->_onChangeCb = NULL;
    this->_onRefreshCb = NULL;
    this->_onDeleteCb = NULL;
}

void Value::post(void)
{
    switch(permission) {
        case READ:
            reportState = new State(this, _wappstoRpc, TYPE_REPORT, this->valueCreated);
            break;
        case WRITE:
            controlState = new State(this, _wappstoRpc, TYPE_CONTROL, this->valueCreated);
            break;
        case READ_WRITE:
            reportState = new State(this, _wappstoRpc, TYPE_REPORT, this->valueCreated);
            controlState = new State(this, _wappstoRpc, TYPE_CONTROL, this->valueCreated);
            break;
    }
    this->_wappstoRpc.postValue(this);
}

bool Value::change(void)
{
    return false;
}

bool Value::report(int data)
{
    this->reportState->timestamp = getUtcTime();
    this->reportState->data = String(data);
    _wappstoRpc.putState(this->reportState);
    return true;
}

bool Value::report(double data)
{
    this->reportState->timestamp = getUtcTime();
    this->reportState->data = String(data);
    _wappstoRpc.putState(this->reportState);
    return true;
}

bool Value::report(const String &data)
{
    this->reportState->timestamp = getUtcTime();
    this->reportState->data = data;
    _wappstoRpc.putState(this->reportState);
    return true;
}

bool Value::control(int data)
{
    this->controlState->timestamp = getUtcTime();
    this->controlState->data = String(data);
    _wappstoRpc.putState(this->controlState);
    return true;
}

bool Value::control(double data)
{
    this->controlState->timestamp = getUtcTime();
    this->controlState->data = String(data);
    _wappstoRpc.putState(this->controlState);
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

String Value::getControlData(void)
{
    if(this->controlState) {
        return this->controlState->data;
    } else {
        return "";
    }
}

double Value::getControlNumberData(void)
{
    if(this->controlState) {
        return this->controlState->data.toDouble();
    } else {
        return 0;
    }
}

String Value::getControlTimestamp(void)
{
    if(this->controlState) {
        return this->controlState->timestamp;
    } else {
        return "";
    }
}

String Value::getReportData(void)
{
    if(this->reportState) {
        return this->reportState->data;
    }  else {
        return "";
    }
}

double Value::getReportNumberData(void)
{
    if(this->reportState) {
        return this->reportState->data.toDouble();
    }  else {
        return 0;
    }
}

String Value::getReportTimestamp(void)
{
    if(this->reportState) {
        return this->reportState->timestamp;
    } else {
        return "";
    }
}

void Value::onControl(WappstoValueControlStringCallback cb)
{
    this->_onControlStringCb = cb;
}

void Value::onControl(WappstoValueControlNumberCallback cb)
{
    this->_onControlNumberCb = cb;
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
