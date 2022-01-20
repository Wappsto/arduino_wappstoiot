#pragma once

#include "WappstoIoT.h"
#include "WappstoRpc.h"
#include "WappstoLog.h"

class WappstoModel;

typedef void (*WappstoDeleteCallback)(WappstoModel *model);
typedef void (*WappstoRefreshCallback)(WappstoModel *model);

class WappstoModel
{
public:
    const char* type;

    WappstoModel(WappstoModel* parent, const char* type);

    const char* getUUID();
    const char* getType();

    bool handleRequest(const char* tmpUuid, RequestType_e req, JsonObject obj);
    void onDelete(WappstoDeleteCallback cb);
    void onRefresh(WappstoRefreshCallback cb);
    void generateUUID();
    bool loadFromWappsto();
    bool create();
    bool update();
    bool fetch();

    virtual void toJSON(JsonObject data) = 0;
    virtual bool handleUpdate(JsonObject obj) = 0;
    virtual bool handleChildren(const char* tmpUuid, RequestType_e req, JsonObject obj) = 0;
    virtual void getFindQuery(char *url) = 0;

protected:
    WappstoRpc *_wappstoRpc;
    WappstoLog *_wappstoLog;
    UUID_t uuid;
    WappstoModel *parent;
    bool newModel;

private:
    void getUrl(char *url);
    WappstoRefreshCallback _onRefreshCb;
    WappstoDeleteCallback _onDeleteCb;
};
