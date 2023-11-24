#include "State.h"

State::State(WappstoModel *parent, StateType_e stateType, bool forceCreate): WappstoModel(parent, "state")
{
    this->_wappstoRpc = WappstoRpc::instance();
    this->parent = parent;
    this->stateType = stateType;
    this->data = "";

    memset(this->timestamp, 0, TIMESTAMP_LENGTH);

    if(forceCreate) {
        generateNewUuid(this->uuid);
    } else {
        if(this->loadFromWappsto()) {
            this->fetch();
            return;
        }
    }
    strcpy(this->timestamp, getUtcTime());
    this->create();
}

const char* State::typeToString() {
    if(this->stateType == TYPE_REPORT) {
        return "Report";
    } else {
        return "Control";
    }
}

void State::toJSON(JsonObject data)
{
    data["timestamp"] = this->timestamp;
    data["data"] = this->data.c_str();
    data["type"] = this->typeToString();
}

bool State::updateRaw(const char *data)
{
    char url[200] = {0,};
    if(strlen(data) < 200) {
        this->data = String(data);
    }
    strcpy(this->timestamp, getUtcTime());
    this->getUrl(url);
    return this->_wappstoRpc->sendRaw(url, data, this->timestamp);
}

void State::getFindQuery(char *url) {
    sprintf(url, "?this_type==%s", this->typeToString());
}

bool State::handleUpdate(JsonObject obj)
{
    this->_wappstoLog->verbose("State update");

    if(obj["data"]) {
        this->data = String((const char*)obj["data"]);
    }
    if(obj["timestamp"]) {
        strcpy(this->timestamp, obj["timestamp"]);
    }

    this->_wappstoLog->verbose("State update data: ", this->data.c_str());
    this->_wappstoLog->verbose("State update timestamp: ", this->timestamp);
    return true;
}

bool State::handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj)
{
    (void)tmpUuid;
    (void)req;
    (void)obj;
    return false;
}

bool State::handleRefresh()
{
    if(this->parent) {
        return this->parent->handleRefresh();
    }
    return false;
}

bool State::handleDelete()
{
    if(this->parent) {
        return this->parent->handleDelete();
    }
    return false;
}