#include "Value.h"


Value::Value(WappstoModel *device, ValueNumber_t *valNumber) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valNumber->name;
    this->type = valNumber->type;
    this->permission = valNumber->permission;
    this->valueType = NUMBER_VALUE;
    this->valNumber = valNumber;
}

Value::Value(WappstoModel *device, ValueString_t *valString) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valString->name;
    this->type = valString->type;
    this->permission = valString->permission;
    this->valueType = STRING_VALUE;
    this->valString = valString;
}

Value::Value(WappstoModel *device, ValueBlob_t *valBlob) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valBlob->name;
    this->type = valBlob->type;
    this->permission = valBlob->permission;
    this->valueType = BLOB_VALUE;
    this->valBlob = valBlob;
}

Value::Value(WappstoModel *device, ValueXml_t *valXml) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valBlob->name;
    this->type = valBlob->type;
    this->permission = valBlob->permission;
    this->valueType = XML_VALUE;
    this->valXml = valXml;
}

void Value::_init(void)
{
    this->valueCreated = false;
    this->reportState = NULL;
    this->controlState = NULL;
    this->_onControlStringCb = NULL;
    this->_onControlNumberCb = NULL;
    this->valNumber = NULL;
    this->valString = NULL;
    this->valBlob = NULL;
    this->valXml = NULL;
    this->_onDeleteCb = NULL;
    this->_onRefreshCb = NULL;
}

void Value::toJSON(JsonObject data)
{
    data["name"] = this->name;
    if(this->permission == READ_WRITE) {
        data["permission"] = "rw";
    } else if(this->permission == READ) {
        data["permission"] = "r";
    } else if(this->permission == WRITE) {
        data["permission"] = "w";
    }

    data["type"] = this->type;

    if(this->valueType == NUMBER_VALUE) {
        JsonObject number = data.createNestedObject("number");
        number["min"] = this->valNumber->min;
        number["max"] = this->valNumber->max;
        number["step"] = this->valNumber->step;
        number["unit"] = "";
    } else if(this->valueType == STRING_VALUE) {
        JsonObject str = data.createNestedObject("string");
        str["max"] = this->valString->max;
        str["encoding"] = this->valString->encoding;
    } else if(this->valueType == BLOB_VALUE) {
        JsonObject blob = data.createNestedObject("blob");
        blob["max"] = this->valBlob->max;
        blob["encoding"] = this->valBlob->encoding;
    } else if(this->valueType == XML_VALUE) {
        JsonObject xml = data.createNestedObject("xml");
        xml["xsd"] = this->valXml->xsd;
        xml["namespace"] = this->valXml->xml_namespace;
    }
}

void Value::createStates(void)
{
    this->_wappstoLog->verbose("Creating states: ", this->permission);
    switch(this->permission) {
        case READ:
            this->reportState = new State(this, TYPE_REPORT, this->valueCreated);
            break;
        case WRITE:
            this->controlState = new State(this, TYPE_CONTROL, this->valueCreated);
            break;
        case READ_WRITE:
            this->reportState = new State(this, TYPE_REPORT, this->valueCreated);
            this->controlState = new State(this, TYPE_CONTROL, this->valueCreated);
            break;
    }
}

bool Value::report(const String &data)
{
    if(this->reportState) {
        strcpy(this->reportState->timestamp, getUtcTime());
        this->reportState->data = data;
        this->reportState->update();
        return true;
    }
    return false;
}

bool Value::report(int data)
{
    return this->report(String(data));
}

bool Value::report(double data)
{
    return this->report(String(data));
}

bool Value::control(const String &data)
{
    if(this->controlState) {
        strcpy(this->controlState->timestamp, getUtcTime());
        this->controlState->data = data;
        this->controlState->update();
        return true;
    }
    return false;
}

bool Value::control(int data)
{
    return this->control(String(data));
}

bool Value::control(double data)
{
    return this->control(String(data));
}

String Value::getControlData(void)
{
    if(this->controlState) {
        return this->controlState->data;
    } else {
        return "";
    }
}

double Value::getControlNumberData(void)
{
    if(this->controlState) {
        return this->controlState->data.toDouble();
    } else {
        return 0;
    }
}

String Value::getControlTimestamp(void)
{
    if(this->controlState) {
        return this->controlState->timestamp;
    } else {
        return "";
    }
}

String Value::getReportData(void)
{
    if(this->reportState) {
        return this->reportState->data;
    }  else {
        return "";
    }
}

double Value::getReportNumberData(void)
{
    if(this->reportState) {
        return this->reportState->data.toDouble();
    }  else {
        return 0;
    }
}

String Value::getReportTimestamp(void)
{
    if(this->reportState) {
        return this->reportState->timestamp;
    } else {
        return "";
    }
}

void Value::onControl(WappstoValueControlStringCallback cb)
{
    this->_onControlStringCb = cb;
}

void Value::onControl(WappstoValueControlNumberCallback cb)
{
    this->_onControlNumberCb = cb;
}

bool Value::handleUpdate(JsonObject obj)
{
    this->_wappstoLog->error("Update of value is not implemeted");
    return false;
}

bool Value::handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj)
{
    if(this->controlState) {
        if(this->controlState->handleRequest(tmpUuid, req, obj)) {
            if(this->_onControlNumberCb) {
                double tmpDouble = this->controlState->data.toDouble();
                this->_onControlNumberCb(this, tmpDouble, this->controlState->timestamp);
            }
            if(this->_onControlStringCb) {
                this->_onControlStringCb(this, this->controlState->data, this->controlState->timestamp);
            }
            return true;
        }
    }
    if(this->reportState) {
        if(this->reportState->handleRequest(tmpUuid, req, obj)) {
            return true;
        }
    }
    return false;
}

void Value::getFindQuery(char *url)
{
    sprintf(url, "?this_name==%s", this->name.c_str());
}

void Value::onRefresh(WappstoValueCallback cb)
{
    this->_onRefreshCb = cb;
}

void Value::onDelete(WappstoValueCallback cb)
{
    this->_onDeleteCb = cb;
}

bool Value::handleRefresh()
{
    if(this->_onRefreshCb) {
        this->_onRefreshCb(this);
        return true;
    }
    return false;
}

bool Value::handleDelete()
{
    if(this->_onDeleteCb) {
        this->_onDeleteCb(this);
        return true;
    }
    return false;
}