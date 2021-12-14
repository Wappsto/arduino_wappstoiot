#include "WappstoIoT.h"



Device::Device(Network *network, WappstoRpc &wappstoRpc, uint8_t id, String name, DeviceDescription_t *deviceInfo) : _wappstoRpc(wappstoRpc)
{
    this->parent = network;
    this->id = id;
    this->name = name;
    this->deviceInfo.product = deviceInfo->product;
    this->deviceInfo.manufacturer = deviceInfo->manufacturer;
    this->deviceInfo.description = deviceInfo->description;
    this->deviceInfo.protocol = deviceInfo->protocol;
    this->deviceInfo.communication = deviceInfo->communication;
    this->deviceInfo.version = deviceInfo->version;
    this->deviceInfo.serial = deviceInfo->serial;

    this->_onChangeCb = NULL;
    this->_onDeleteCb = NULL;

    currentNumberOfValues = 0;
}

void Device::post(void)
{
    this->_wappstoRpc.postDevice(this);
}

bool Device::change(void)
{
    return false;
}

bool Device::deleteReq(void)
{
    return false;
}

Value* Device::createValueNumber(String name, String type, PERMISSION_e permission, ValueNumber_t *valNumber)
{
    if(currentNumberOfValues >= MAX_VALUES) {
        Serial.println("Cannot create more values");
        return NULL;
    }
    currentNumberOfValues++;
    values[currentNumberOfValues-1] = new Value(this, _wappstoRpc, currentNumberOfValues-1, name, type, permission, valNumber);

    if(!_wappstoRpc.getValueUuidFromName(this, name, values[currentNumberOfValues-1]->uuid)) {
        generateNewUuid(values[currentNumberOfValues-1]->uuid);
        values[currentNumberOfValues-1]->valueCreated = true;
    }

    values[currentNumberOfValues-1]->post();
    return values[currentNumberOfValues-1];
}

Value* Device::createValueString(String name, String type, PERMISSION_e permission, ValueString_t *valString)
{
    if(currentNumberOfValues >= MAX_VALUES) {
        Serial.println("Cannot create more values");
        return NULL;
    }
    currentNumberOfValues++;
    values[currentNumberOfValues-1] = new Value(this, _wappstoRpc, currentNumberOfValues-1, name, type, permission, valString);

    if(!_wappstoRpc.getValueUuidFromName(this, name, values[currentNumberOfValues-1]->uuid)) {
        generateNewUuid(values[currentNumberOfValues-1]->uuid);
    }

    values[currentNumberOfValues-1]->post();
    return values[currentNumberOfValues-1];
}

void Device::onChange(WappstoCallback cb)
{
    this->_onChangeCb = cb;
}

void Device::onDelete(WappstoCallback cb)
{
    this->_onDeleteCb = cb;
}
