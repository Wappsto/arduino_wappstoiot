#include "WappstoIoT.h"


Value::Value(Device *device, ValueNumber_t *valNumber) : parent(device)
{
    _init();
    this->name = valNumber->name;
    this->type = valNumber->type;
    this->permission = valNumber->permission;
    this->valueType = NUMBER_VALUE;
    this->valNumber = valNumber;
    this->valString = NULL;
}

Value::Value(Device *device, ValueString_t *valString) : parent(device)
{
    _init();
    this->name = valString->name;
    this->type = valString->type;
    this->permission = valString->permission;
    this->valueType = STRING_VALUE;
    this->valNumber = NULL;
    this->valString = valString;
}

Value::Value(Device *device, ValueBlob_t *valBlob) : parent(device)
{
    _init();
    this->name = valBlob->name;
    this->type = valBlob->type;
    this->permission = valBlob->permission;
    this->valueType = BLOB_VALUE;
    this->valNumber = NULL;
    this->valString = NULL;
    this->valBlob = valBlob;
}

void Value::_init(void)
{
    _wappstoRpc = WappstoRpc::instance();
    this->valueCreated = false;
    this->reportState = NULL;
    this->controlState = NULL;
    this->_onControlStringCb = NULL;
    this->_onControlNumberCb = NULL;
    this->_onRefreshCb = NULL;
    this->_onDeleteCb = NULL;
}

void Value::post(void)
{
    switch(permission) {
        case READ:
            reportState = new State(this, TYPE_REPORT, this->valueCreated);
            break;
        case WRITE:
            controlState = new State(this, TYPE_CONTROL, this->valueCreated);
            break;
        case READ_WRITE:
            reportState = new State(this, TYPE_REPORT, this->valueCreated);
            controlState = new State(this, TYPE_CONTROL, this->valueCreated);
            break;
    }
    this->_wappstoRpc->postValue(this);
}

bool Value::report(int data)
{
    if(this->reportState) {
        this->reportState->timestamp = getUtcTime();
        this->reportState->data = String(data);
        _wappstoRpc->putState(this->reportState);
        return true;
    }
    return false;
}

bool Value::report(double data)
{
    if(this->reportState) {
        this->reportState->timestamp = getUtcTime();
        this->reportState->data = String(data);
        _wappstoRpc->putState(this->reportState);
        return true;
    }
    return false;
}

bool Value::report(const String &data)
{
    if(this->reportState) {
        this->reportState->timestamp = getUtcTime();
        this->reportState->data = data;
        _wappstoRpc->putState(this->reportState);
        return true;
    }
    return false;
}

bool Value::control(int data)
{
    if(this->controlState) {
        this->controlState->timestamp = getUtcTime();
        this->controlState->data = String(data);
        _wappstoRpc->putState(this->controlState);
        return true;
    }
    return false;
}

bool Value::control(double data)
{
    if(this->controlState) {
        this->controlState->timestamp = getUtcTime();
        this->controlState->data = String(data);
        _wappstoRpc->putState(this->controlState);
        return true;
    }
    return false;
}

bool Value::control(const String &data)
{
    if(this->controlState) {
        this->controlState->timestamp = getUtcTime();
        this->controlState->data = data;
        _wappstoRpc->putState(this->controlState);
        return true;
    }
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

void Value::onRefresh(WappstoValueRefreshCallback cb)
{
    this->_onRefreshCb = cb;
}

void Value::onDelete(WappstoCallback cb)
{
    this->_onDeleteCb = cb;
}
