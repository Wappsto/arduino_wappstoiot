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
    if(currentNumberOfValues >= MAX_VALUES) {
        this->_wappstoLog->error("Cannot create more values");
        return NULL;
    }
    currentNumberOfValues++;
    values[currentNumberOfValues-1] = new Value(this, valNumber);

    if(!this->_wappstoRpc->getValueUuidFromName(this, valNumber->name, values[currentNumberOfValues-1]->uuid)) {
        generateNewUuid(values[currentNumberOfValues-1]->uuid);
        values[currentNumberOfValues-1]->valueCreated = true;
    }

    values[currentNumberOfValues-1]->post();
    return values[currentNumberOfValues-1];
}

Value* Device::createValueString(ValueString_t *valString)
{
    if(currentNumberOfValues >= MAX_VALUES) {
        _wappstoLog->error("Cannot create more values");
        return NULL;
    }
    currentNumberOfValues++;
    values[currentNumberOfValues-1] = new Value(this, valString);
    if(!this->_wappstoRpc->getValueUuidFromName(this, valString->name, values[currentNumberOfValues-1]->uuid)) {
        generateNewUuid(values[currentNumberOfValues-1]->uuid);
    }
    values[currentNumberOfValues-1]->post();
    return values[currentNumberOfValues-1];
}

Value* Device::createValueBlob(ValueBlob_t *valBlob)
{
    if(currentNumberOfValues >= MAX_VALUES) {
        this->_wappstoLog->error("Cannot create more values");
        return NULL;
    }
    currentNumberOfValues++;
    values[currentNumberOfValues-1] = new Value(this, valBlob);

    if(!this->_wappstoRpc->getValueUuidFromName(this, valBlob->name, values[currentNumberOfValues-1]->uuid)) {
        generateNewUuid(values[currentNumberOfValues-1]->uuid);
    }

    values[currentNumberOfValues-1]->post();
    return values[currentNumberOfValues-1];
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

Value* Device::createValueXml(ValueXml_t *valXml)
{
    Value** value = this->getFreeValue();
    *value = new Value(this, valXml);

    if(!this->_wappstoRpc->getValueUuidFromName(this, valXml->name, (*values)->uuid)) {
        generateNewUuid((*value)->uuid);
    }

    *value->post();
    return *value;
}

void Device::onDelete(WappstoCallback cb)
{
    this->_onDeleteCb = cb;
}
