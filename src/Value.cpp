#include <time.h>
#include <sys/time.h>

#include "Value.h"

#define MINIMUM_PERIOD_JITTER_SEC 10

Value::Value(WappstoModel *device, ValueNumber_t *valNumber) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valNumber->name;
    this->type = valNumber->type;
    this->permission = valNumber->permission;
    this->valueType = NUMBER_VALUE;
    this->valNumber = valNumber;
    this->delta = 0;
    this->period = 0;
}

Value::Value(WappstoModel *device, ValueNumberFull_t *valNumber) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valNumber->name;
    this->type = valNumber->type;
    this->permission = valNumber->permission;
    this->valueType = NUMBER_VALUE_FULL;
    this->valNumberFull = valNumber;
    this->delta = valNumber->delta.toDouble();
    this->period = valNumber->period.toInt();
}

Value::Value(WappstoModel *device, ValueString_t *valString) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valString->name;
    this->type = valString->type;
    this->permission = valString->permission;
    this->valueType = STRING_VALUE;
    this->valString = valString;
    this->delta = 0;
    this->period = 0;
}

Value::Value(WappstoModel *device, ValueBlob_t *valBlob) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valBlob->name;
    this->type = valBlob->type;
    this->permission = valBlob->permission;
    this->valueType = BLOB_VALUE;
    this->valBlob = valBlob;
    this->delta = 0;
    this->period = 0;
}

Value::Value(WappstoModel *device, ValueXml_t *valXml) : WappstoModel(device, "value")
{
    this->_init();
    this->name = valXml->name;
    this->type = valXml->type;
    this->permission = valXml->permission;
    this->valueType = XML_VALUE;
    this->valXml = valXml;
    this->delta = 0;
    this->period = 0;
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
    this->reportTriggerType = USER_REPORT;
    this->jitterTriggerUnixTime = 0;
    this->jitterData = "";
    memset(this->jitterTimestamp, 0, TIMESTAMP_LENGTH);
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
    data["period"] = "0";
    data["delta"] = "0";

    if(this->valueType == NUMBER_VALUE) {
        JsonObject number = data.createNestedObject("number");
        number["min"] = this->valNumber->min;
        number["max"] = this->valNumber->max;
        number["step"] = this->valNumber->step;
        number["unit"] = this->valNumber->unit;
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
    } else if(this->valueType == NUMBER_VALUE_FULL) {
        JsonObject number = data.createNestedObject("number");
        number["min"] = this->valNumberFull->min;
        number["max"] = this->valNumberFull->max;
        number["step"] = this->valNumberFull->step;
        number["unit"] = this->valNumberFull->unit;
        //number["meaningful_zero"] = this->valNumberFull->meaningful_zero;
        number["ordered_mapping"] = this->valNumberFull->ordered_map;
        if(this->valNumberFull->mapping) {
            JsonObject mapping = number.createNestedObject("mapping");
            for(int i = 0; i< this->valNumberFull->mapping->size; i++) {
                mapping[this->valNumberFull->mapping->map[i].key] = this->valNumberFull->mapping->map[i].value;
            }
        }
        data["period"] = this->valNumberFull->period;
        data["delta"] = this->valNumberFull->delta;
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
        if((this->reportTriggerType == PERIOD_REPORT) && (this->period > MINIMUM_PERIOD_JITTER_SEC)) {
            time_t time_now = time(NULL);
            this->_wappstoLog->warning("STORINTG JITTER: ", (int)time_now);
            strcpy(this->jitterTimestamp, getUtcTime());
            this->jitterData = data;
            this->jitterTriggerUnixTime = time_now + (int)(random(MINIMUM_PERIOD_JITTER_SEC));
            this->reportTriggerType = USER_REPORT;
        } else {
            this->reportTriggerType = USER_REPORT;
            strcpy(this->reportState->timestamp, getUtcTime());
            this->reportState->data = data;
            this->reportState->update();
            return true;
        }
    }
    return false;
}

bool Value::report(int data)
{
    if(!this->reportState) {
        return false;
    }
    if(this->reportTriggerType == USER_REPORT) {
        if(isinf(this->delta)) {
            this->_wappstoLog->verbose("Delta (int) is infinite, ignore report: ", this->delta);
            return false;
        }
        if(this->delta > 0) {
            if(abs(this->reportState->data.toDouble() - data) > this->delta) {
                this->_wappstoLog->verbose("Rejected report due to delta, diff: ", (abs(this->reportState->data.toDouble() - data) > this->delta));
                return false;
            }
        }
    }
    return this->report(String(data));
}

bool Value::report(double data)
{
    uint8_t count = 0;
    if(!this->reportState) {
        return false;
    }
    if(this->reportTriggerType == USER_REPORT) {
        if(isinf(this->delta)) {
            this->_wappstoLog->verbose("Delta (double) is infinite, ignore report: ", this->delta);
            return false;
        }
        if(this->delta > 0) {
            if(fabs(this->reportState->data.toDouble() - data) > this->delta) {
                this->_wappstoLog->verbose("Rejected report due to delta, diff: ", (abs(this->reportState->data.toDouble() - data) > this->delta));
                return false;
            }
        }
    }

    if(this->valueType == NUMBER_VALUE) {
        double num = this->valNumber->step;
        num = fabs(num);
        num = num - (int)num;
        while (fabs(num) >= 0.0000001) {
            num = num * 10;
            count = count + 1;
            num = num - int(num);
        }

        if(count > 0) {
            return this->report(String(data, count));
        }
    }
    return this->report(String(data));
}

bool Value::report(const char* data)
{
    if(this->reportState) {
        return this->reportState->updateRaw(data);
    } else {
        return false;
    }
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
    uint8_t count = 0;
    if(this->valueType == NUMBER_VALUE) {
        double num = this->valNumber->step;
        num = fabs(num);
        num = num - (int)num;
        while (fabs(num) >= 0.0000001) {
            num = num * 10;
            count = count + 1;
            num = num - int(num);
        }
        if(count > 0) {
            return this->control(String(data, count));
        }
    }
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

void Value::calculateNextPeriodTrigger(void)
{
    struct tm loc_time = { };
    struct tm midnight = { };
    time_t time_now = time(NULL);

    if(this->period == 0) {
        this->nextTriggerUnixTime = 0;
        return;
    }

    localtime_r(&time_now, &loc_time);

    midnight.tm_year = loc_time.tm_year;
    midnight.tm_mon = loc_time.tm_mon;
    midnight.tm_mday = loc_time.tm_mday;
    midnight.tm_hour = 0;
    midnight.tm_min = 0;
    midnight.tm_sec = 0;
    midnight.tm_isdst = loc_time.tm_isdst;

    time_t time_midnight;
    time_midnight = mktime(&midnight);
    int seconds_since_midnight = (int)difftime(time_now, time_midnight);

    int seconds_to_next_period = ((seconds_since_midnight / this->period) + 1) * this->period;
    int next_trigger_in_seconds = seconds_to_next_period - seconds_since_midnight;
    this->nextTriggerUnixTime = time_now + next_trigger_in_seconds;
}

void Value::handlePeriod(void)
{
    time_t time_now = time(NULL);

    if(this->period == 0) {
        this->nextTriggerUnixTime = 0;
        return;
    }
    if(!this->reportState) {
        return;
    }

    if(this->jitterTriggerUnixTime > 0 && this->jitterTriggerUnixTime < time_now) {
        strcpy(this->reportState->timestamp, this->jitterTimestamp);
        this->reportState->data = this->jitterData;
        this->reportState->update();
        this->jitterTriggerUnixTime = 0;
    }

    if(this->nextTriggerUnixTime <= time_now) {
        this->reportTriggerType = PERIOD_REPORT;
        this->handleRefresh();
        this->calculateNextPeriodTrigger();
    }
}

bool Value::handleUpdate(JsonObject obj)
{
    if(obj["delta"]) {
        this->delta = atof((const char*)obj["delta"]);
    }
    if(obj["period"]) {
        this->period = atoi((const char*)obj["period"]);
    }
    this->calculateNextPeriodTrigger();
    return true;
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
        if(this->reportTriggerType != PERIOD_REPORT) {
            this->reportTriggerType = REFRESH_REPORT;
        }
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
