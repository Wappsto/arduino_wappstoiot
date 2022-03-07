#include "Device.h"

Device::Device(WappstoModel *network, DeviceDescription_t *deviceInfo): WappstoModel(network, "device")
{
    this->name = deviceInfo->name;
    this->deviceInfo.product = deviceInfo->product;
    this->deviceInfo.manufacturer = deviceInfo->manufacturer;
    this->deviceInfo.description = deviceInfo->description;
    this->deviceInfo.protocol = deviceInfo->protocol;
    this->deviceInfo.communication = deviceInfo->communication;
    this->deviceInfo.version = deviceInfo->version;
    this->deviceInfo.serial = deviceInfo->serial;

    this->currentNumberOfValues = 0;
    this->_onDeleteCb = NULL;
    this->_onRefreshCb = NULL;
}

void Device::toJSON(JsonObject data)
{
    data["name"] = this->name;
    data["manufacturer"] = this->deviceInfo.manufacturer;
    data["product"] = this->deviceInfo.product;
    data["version"] = this->deviceInfo.version;
    data["serial"] = this->deviceInfo.serial;
    data["description"] = this->deviceInfo.description;
    data["protocol"] = this->deviceInfo.protocol;
    data["communication"] = this->deviceInfo.communication;
}

void Device::getFindQuery(char *url)
{
    sprintf(url, "?this_name==%s", this->name.c_str());
}

Value* Device::sendValue(Value* value)
{
    value->loadFromWappsto();
    if(value->create()) {
        value->createStates();
        return value;
    } else {
        this->_wappstoLog->error("Failed to create Value, error from server");
        return NULL;
    }
}

Value* Device::createValueNumber(ValueNumber_t *valNumber)
{
    Value** value = this->getFreeValue();
    if(value == NULL) {
        return NULL;
    }

    *value = new Value(this, valNumber);
    return this->sendValue(*value);
}

Value* Device::createValueNumber(ValueNumberFull_t *valNumber)
{
    Value** value = this->getFreeValue();
    if(value == NULL) {
        return NULL;
    }

    *value = new Value(this, valNumber);
    return this->sendValue(*value);
}

Value* Device::createValueString(ValueString_t *valString)
{
    Value** value = this->getFreeValue();
    if(value == NULL) {
        return NULL;
    }

    *value = new Value(this, valString);
    return this->sendValue(*value);
}

Value* Device::createValueBlob(ValueBlob_t *valBlob)
{
    Value** value = this->getFreeValue();
    if(value == NULL) {
        return NULL;
    }

    *value = new Value(this, valBlob);
    return this->sendValue(*value);
}

Value* Device::createValueXml(ValueXml_t *valXml)
{
    Value** value = this->getFreeValue();
    if(value == NULL) {
        return NULL;
    }

    *value = new Value(this, valXml);
    return this->sendValue(*value);
}

Value** Device::getFreeValue()
{
    if(this->currentNumberOfValues >= MAX_VALUES) {
        this->_wappstoLog->error("Cannot create more values");
        return NULL;
    }

    return &this->values[this->currentNumberOfValues++];
}

bool Device::handleUpdate(JsonObject obj) {
    (void)obj;
    return false;
}

bool Device::handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj) {
    for(int vals=0; vals < this->currentNumberOfValues; vals++) {
        if(this->values[vals] == NULL) {
            continue;
        }

        bool res = this->values[vals]->handleRequest(tmpUuid, req, obj);
        if(res) {
            return res;

        }
    }

    return false;
}

void Device::onRefresh(WappstoDeviceCallback cb)
{
    this->_onRefreshCb = cb;
}

void Device::onDelete(WappstoDeviceCallback cb)
{
    this->_onDeleteCb = cb;
}

bool Device::handleRefresh()
{
    if(this->_onRefreshCb) {
        this->_onRefreshCb(this);
        return true;
    }
    return false;
}

bool Device::handleDelete()
{
    if(this->_onDeleteCb) {
        this->_onDeleteCb(this);
        return true;
    }
    return false;
}