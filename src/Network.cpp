#include "Network.h"

Network::Network(const char* uuid, String name, String description): WappstoModel{NULL, "network"}
{
    strcpy(this->uuid, uuid);
    this->name = name;
    this->description = description;
    this->currentNumberOfDevices = 0;
    this->_onDeleteCb = NULL;
    this->_onRefreshCb = NULL;
    this->newModel = true;
}

void Network::toJSON(JsonObject data)
{
    data["name"] = this->name.c_str();
    if(this->description) {
        data["description"] = this->description.c_str();
    }
}

Device* Network::createDevice(DeviceDescription_t *deviceInfo)
{
    if(this->currentNumberOfDevices >= MAX_DEVICES) {
        this->_wappstoLog->error("Cannot create more devices");
        return NULL;
    }
    Device *device = new Device(this, deviceInfo);
    this->devices[this->currentNumberOfDevices++] = device;

    device->loadFromWappsto();
    if(device->create()) {
        return device;
    } else {
        this->_wappstoLog->error("Failed to create device, error from server");
        return NULL;
    }

}

void Network::handlePeriod(void) {
    for(int devs=0; devs < this->currentNumberOfDevices; devs++) {
        if(this->devices[devs] == NULL) {
            continue;
        }
        this->devices[devs]->handlePeriod();
    }
}

bool Network::handleUpdate(JsonObject obj) {
    (void)obj;
    this->_wappstoLog->error("Update on network not supported");
    return false;
}

bool Network::handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj) {
    for(int devs=0; devs < this->currentNumberOfDevices; devs++) {
        if(this->devices[devs] == NULL) {
            continue;
        }

        bool res = this->devices[devs]->handleRequest(tmpUuid, req, obj);
        if(res) {
            return res;

        }
    }

    return false;
}

void Network::getFindQuery(char *url)
{
    (void)url;
}

void Network::onRefresh(WappstoNetworkCallback cb)
{
    this->_onRefreshCb = cb;
}

void Network::onDelete(WappstoNetworkCallback cb)
{
    this->_onDeleteCb = cb;
}

bool Network::handleRefresh()
{
    if(this->_onRefreshCb) {
        this->_onRefreshCb(this);
        return true;
    }
    return false;
}

bool Network::handleDelete()
{
    if(this->_onDeleteCb) {
        this->_onDeleteCb(this);
        return true;
    }
    return false;
}
