#include "WappstoModel.h"

WappstoModel::WappstoModel(WappstoModel *parent, const char *type) {
    this->_wappstoRpc = WappstoRpc::instance();
    this->_wappstoLog = WappstoLog::instance();
    this->type = type;
    this->parent = parent;
    this->newModel = false;
    memset(this->uuid, 0, UUID_LENGTH);
}

bool WappstoModel::handleRequest(const char* tmpUuid, RequestType_e req, JsonObject obj) {
    if(strcmp(this->uuid, tmpUuid) == 0) {
        switch(req) {
            case REQUEST_GET:
                return this->handleRefresh();
            case REQUEST_PUT:
                return this->handleUpdate(obj);
            case REQUEST_DELETE:
                return this->handleDelete();
                return false;
        }

        return false;
    }

    return this->handleChildren(tmpUuid, req, obj);
}

const char* WappstoModel::getUUID()
{
    return this->uuid;
}

const char* WappstoModel::getType()
{
    return this->type;
}

void WappstoModel::generateUUID()
{
    this->newModel = true;
    generateNewUuid(this->uuid);
}

void WappstoModel::getUrl(char *url)
{
    if(!this->newModel && strlen(this->uuid) > 0) {
        sprintf(url, "/%s/%s", this->type, this->uuid);
    } else if(this->parent) {
        sprintf(url, "/%s/%s/%s", this->parent->getType(), this->parent->getUUID(), this->type);
    } else {
        sprintf(url, "/%s", this->type);
    }
}

bool WappstoModel::loadFromWappsto()
{
    if(!this->parent) {
        this->_wappstoLog->error("Can't load data from wappsto, when there is no parent");
        return false;
    }

    char url[200] = {0,};
    this->getUrl(url);
    this->getFindQuery(&url[strlen(url)]);

    this->_wappstoRpc->generateRPCRequest("GET", url, false);

    StaticJsonDocument<JSON_STATIC_BUFFER_SIZE> doc;
    bool res = this->_wappstoRpc->send(&doc);

    if(res) {
        if(doc["result"]["value"]["id"].size() > 0) {
            strcpy(this->uuid, doc["result"]["value"]["id"][0]);
            return true;
        }
    }

    this->generateUUID();
    return false;
}

bool WappstoModel::create()
{
    char url[200] = {0,};
    this->getUrl(url);

    JsonObject data;
    if(!this->newModel && strlen(this->uuid)) {
        data = this->_wappstoRpc->generateRPCRequest("PUT", url, true);
    } else {
        data = this->_wappstoRpc->generateRPCRequest("POST", url, true);
    }
    this->newModel = false;

    this->toJSON(data);

    JsonObject meta = data.createNestedObject("meta");
    meta["id"] = this->uuid;
    meta["type"] = this->type;
    meta["version"] = "2.0";

    return this->_wappstoRpc->send();
}

bool WappstoModel::update()
{
    char url[200] = {0,};
    this->getUrl(url);

    JsonObject data = this->_wappstoRpc->generateRPCRequest("PUT", url, true);

    this->toJSON(data);

    return this->_wappstoRpc->send();
}

bool WappstoModel::fetch()
{
    char url[200] = {0,};
    this->getUrl(url);

    JsonObject data = this->_wappstoRpc->generateRPCRequest("GET", url, false);

    StaticJsonDocument<JSON_STATIC_BUFFER_SIZE> doc;
    if(this->_wappstoRpc->send(&doc)) {
        if(doc["result"] && doc["result"]["value"]) {
            this->handleUpdate(doc["result"]["value"]);
            return true;
        }
    }
    return false;
}