#pragma once

#ifndef WiFiClientSecure_h
#define JsonMockContainer_h

#include "Arduino.h"

#define MAX_EVENTS 25

class JsonMockContainer
{
public:
    JsonMockContainer();
    ~JsonMockContainer();

    void addExpectRequest(const char* request);
    void addResponse(const char* response);
    void addEvent(const char* event);
    void clearAll(void);
    const char* getNextEvent(void);
    char* getNextResponse(void);

    char* requestList[MAX_EVENTS];
    char* responseList[MAX_EVENTS];
    char* eventList[MAX_EVENTS];

    int numberRequests;
    int numberResponses;
    int numberEvents;
    int currentRequest;
    int currentResponse;
    int currentEvent;
};

#endif
