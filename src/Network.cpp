#include "WappstoIoT.h"
#include "WappstoRpc.h"

Network::Network(const char* uuid, String name, String description)
{
    this->_wappstoRpc = WappstoRpc::instance();
    this->_wappstoLog = WappstoLog::instance();
    this->name = name;
    strcpy(this->uuid, uuid);
    this->description = description;
    this->currentNumberOfDevices = 0;
}

void Network::post(void)
{
    this->_wappstoRpc->postNetwork(this->uuid, this->name);
}

Device* Network::createDevice(DeviceDescription_t *deviceInfo)
{
    if(this->currentNumberOfDevices >= MAX_DEVICES) {
        this->_wappstoLog->error("Cannot create more devices");
        return NULL;
    }
    this->currentNumberOfDevices++;
    this->devices[this->currentNumberOfDevices-1] = new Device(this, deviceInfo);

    if(!this->_wappstoRpc->getDeviceUuidFromName(this, deviceInfo->name, this->devices[this->currentNumberOfDevices-1]->uuid)) {
            generateNewUuid(this->devices[this->currentNumberOfDevices-1]->uuid);
    }

    this->devices[this->currentNumberOfDevices-1]->post();
    return this->devices[this->currentNumberOfDevices-1];
}

void Network::onDelete(WappstoNetworkDeleteCallback cb)
{
    this->_onDeleteCb = cb;
}
