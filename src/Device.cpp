#include "WappstoIoT.h"


Device::Device(Network *network, DeviceDescription_t *deviceInfo)
{
    this->_wappstoRpc = WappstoRpc::instance();
    this->_wappstoLog = WappstoLog::instance();
    this->parent = network;
    this->name = deviceInfo->name;
    this->deviceInfo.product = deviceInfo->product;
    this->deviceInfo.manufacturer = deviceInfo->manufacturer;
    this->deviceInfo.description = deviceInfo->description;
    this->deviceInfo.protocol = deviceInfo->protocol;
    this->deviceInfo.communication = deviceInfo->communication;
    this->deviceInfo.version = deviceInfo->version;
    this->deviceInfo.serial = deviceInfo->serial;

    this->_onDeleteCb = NULL;

    currentNumberOfValues = 0;
}

void Device::post(void)
{
    this->_wappstoRpc->postDevice(this);
}

Value* Device::createValueNumber(ValueNumber_t *valNumber)
{
    Value** value = this->getFreeValue();
    *value = new Value(this, valNumber);

    postValue(*value, valNumber->name);

    return *value;
}

Value* Device::createValueString(ValueString_t *valString)
{
    Value** value = this->getFreeValue();
    *value = new Value(this, valString);

    postValue(*value, valString->name);

    return *value;
}

Value* Device::createValueBlob(ValueBlob_t *valBlob)
{
    Value** value = this->getFreeValue();
    *value = new Value(this, valBlob);

    postValue(*value, valBlob->name);

    return *value;
}

Value* Device::createValueXml(ValueXml_t *valXml)
{
    Value** value = this->getFreeValue();
    *value = new Value(this, valXml);

    postValue(*value, valXml->name);

    return *value;
}

Value** Device::getFreeValue()
{
    if(currentNumberOfValues >= MAX_VALUES) {
        this->_wappstoLog->error("Cannot create more values");
        return NULL;
    }

    currentNumberOfValues++;
    return &values[currentNumberOfValues-1];
}

void Device::postValue(Value *value, String name)
{
    if(!this->_wappstoRpc->getValueUuidFromName(this, name, value->uuid)) {
        generateNewUuid(value->uuid);
        value->valueCreated = true;
    }

    value->post();
}

void Device::onDelete(WappstoCallback cb)
{
    this->_onDeleteCb = cb;
}
