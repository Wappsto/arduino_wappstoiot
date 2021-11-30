#include "WappstoIoT.h"



/**
Device model
Device::Device(void)
{

}
*/

Device::Device(Network *network, WappstoRpc &wappstoRpc, uint8_t id, String name, String product, String manufacturer, String description, String protocol, String communication) : _wappstoRpc(wappstoRpc)
{
    this->parent = network;
    this->id = id;
    this->name = name;
    this->deviceInfo.product = product;
    this->deviceInfo.manufacturer = manufacturer;
    this->deviceInfo.description = description;
    this->deviceInfo.protocol = protocol;
    this->deviceInfo.communication = communication;
    this->deviceInfo.version = "NA";
    this->deviceInfo.serial = "1";

    this->_onChangeCb = NULL;
    this->_onRefreshCb = NULL;
    this->_onRequestCb = NULL;
    this->_onDeleteCb = NULL;

    currentNumberOfValues = 0;

    uint32_t readId  = (uint32_t) (((uint32_t)0x00 << 24) |
                                   ((uint32_t)id << 16) |
                                   ((uint32_t)0xFF << 8) |
                                   ((uint32_t)0xFF));

    if(readUuid(readId) != NULL) {
        strcpy(this->uuid, readUuid(readId));
    } else {
        generateNewUuid(this->uuid);
    }
}

void Device::post(void)
{
    this->_wappstoRpc.postDevice(this);
}

bool Device::change(void)
{
    return true;
}

bool Device::deleteReq(void)
{
    return true;
}

Value* Device::createValueNumber(String name, String type, PERMISSION_e permission, ValueNumber_t *valNumber)
{
    if(currentNumberOfValues >= MAX_VALUES) {
        Serial.println("Cannot create more values");
        return NULL;
    }
    currentNumberOfValues++;
    values[currentNumberOfValues-1] = new Value(this, _wappstoRpc, currentNumberOfValues-1, name, type, permission, valNumber);
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
    values[currentNumberOfValues-1]->post();
    return values[currentNumberOfValues-1];
}

void Device::onChange(WappstoIoTCallback cb)
{
    this->_onChangeCb = cb;
}

void Device::onRefresh(WappstoIoTCallback cb)
{
    this->_onRefreshCb = cb;
}

void Device::onRequest(WappstoIoTCallback cb)
{
    this->_onRequestCb = cb;
}

void Device::onDelete(WappstoIoTCallback cb)
{
    this->_onDeleteCb = cb;
}
