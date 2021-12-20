#include "WappstoIoT.h"
#include "WappstoRpc.h"

Network::Network(const char* uuid, String name, String description)
{
    _wappstoRpc = WappstoRpc::instance();
    _wappstoLog = WappstoLog::instance();
    this->name = name;
    strcpy(this->uuid, uuid);
    this->description = description;
    currentNumberOfDevices = 0;
}

void Network::post(void)
{
    _wappstoRpc->postNetwork(this->uuid, this->name);
}

bool Network::change(void)
{
    return false;
}

bool Network::deleteReq(void)
{
    return false;
}

Device* Network::createDevice(DeviceDescription_t *deviceInfo)
{
    if(currentNumberOfDevices >= MAX_DEVICES) {
        _wappstoLog->error("Cannot create more devices");
        return NULL;
    }
    currentNumberOfDevices++;
    devices[currentNumberOfDevices-1] = new Device(this, deviceInfo);

    if(!_wappstoRpc->getDeviceUuidFromName(this, deviceInfo->name, devices[currentNumberOfDevices-1]->uuid)) {
            generateNewUuid(devices[currentNumberOfDevices-1]->uuid);
    }

    devices[currentNumberOfDevices-1]->post();
    return devices[currentNumberOfDevices-1];
}

void Network::onChange(WappstoCallback cb)
{
    this->_onChangeCb = cb;
}

void Network::onDelete(WappstoNetworkDeleteCallback cb)
{
    this->_onDeleteCb = cb;
}
