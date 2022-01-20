#pragma once

#include "WappstoIoT.h"
#include "WappstoRpc.h"
#include "WappstoLog.h"

class WappstoModel
{
public:
    const char* type;

    WappstoModel(WappstoModel* parent, const char* type);

    const char* getUUID();
    const char* getType();

    bool handleRequest(const char* tmpUuid, RequestType_e req, JsonObject obj);
    void generateUUID();
    bool loadFromWappsto();
    bool create();
    bool update();
    bool fetch();

    virtual bool handleRefresh() = 0;
    virtual bool handleDelete() = 0;

protected:
    virtual bool handleUpdate(JsonObject obj) = 0;
    virtual bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj) = 0;
    virtual void getFindQuery(char *url) = 0;
    virtual void toJSON(JsonObject data) = 0;

protected:
    WappstoRpc *_wappstoRpc;
    WappstoLog *_wappstoLog;
    UUID_t uuid;
    WappstoModel *parent;
    bool newModel;

private:
    void getUrl(char *url);
};
