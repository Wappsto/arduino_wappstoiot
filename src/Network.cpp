#include "WappstoIoT.h"
#include "WappstoRpc.h"

Network::Network(WappstoRpc &wappstoRpc, const char* uuid, String name, String description) : _wappstoRpc(wappstoRpc)
{
    this->name = name;
    strcpy(this->uuid, uuid);
    this->description = description;
    currentNumberOfDevices = 0;
}

void Network::post(void)
{
    _wappstoRpc.postNetwork(this->uuid, this->name);
}

bool Network::change(void)
{
    return false;
}

bool Network::deleteReq(void)
{
    return false;
}

Device* Network::createDevice(String name, String product, String manufacturer, String description, String protocol, String communication)
{
    if(currentNumberOfDevices >= MAX_DEVICES) {
        Serial.println("Cannot create more devices");
        return NULL;
    }
    currentNumberOfDevices++;
    devices[currentNumberOfDevices-1] = new Device(this, _wappstoRpc, currentNumberOfDevices-1, name, product, manufacturer, description, protocol, communication);
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
