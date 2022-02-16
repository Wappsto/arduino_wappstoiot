#pragma once

#include "Arduino.h"
#include "ArduinoJson.h"

#define MAX_EVENTS 25
#define MAX_DEVICES 5
#define MAX_VALUES 10
#define MAX_STATES MAX_VALUES

class JsonMockContainer
{
public:
    JsonMockContainer();
    ~JsonMockContainer();

    void addDeviceUuid(const char* uuid);
    void addValueUuid(const char* uuid);
    void addStateUuid(const char* uuid, const char* data, bool isReport);

    void testRefresh(const char* uuid, const char* url, char* returnBuffer);
    void testControl(const char* uuid, const char* url, const char* data, char* returnBuffer);

    bool receiveData(const char* data, char* returnBuffer);

private:
    void _clearAll(void);
    char* _getNextDeviceUuid(void);
    char* _getNextValueUuid(void);
    char* _getNextReportStateUuid(void);
    char* _getNextControlStateUuid(void);
    void _findStateAndSendResponse(int msgId, const char* urlStr, char* returnBuffer);

    void _sendResponse(int msgId, char* returnBuffer, bool success);
    void _sendStateData(int msgId, const char* uuid, bool isReport, const char* data, char* returnBuffer);
    void _getDeviceResponse(const char* state, int msgId, char* uuid, char* returnBuffer, bool success);
    bool _verfifyStateId(const char* urlStr);

    char* _deviceUuidList[MAX_DEVICES];
    char* _valueUuidList[MAX_VALUES];
    char* _stateReportUuidList[MAX_STATES];
    char* _stateControlUuidList[MAX_STATES];
    char* _stateReportDataList[MAX_STATES];
    char* _stateControlDataList[MAX_STATES];

    // total
    int _numberDevices;
    int _numberValues;
    int _numberReportStates;
    int _numberControlStates;

    //current
    int _currentDevice;
    int _currentValue;
    int _currentReportState;
    int _currentControlState;
};
