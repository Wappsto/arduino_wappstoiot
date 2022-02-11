#include "JsonMockContainer.h"

JsonMockContainer::JsonMockContainer()
{
    Serial.println("JsonMockContainer!!!: Constructor");
    this->clearAll();
}

JsonMockContainer::~JsonMockContainer()
{
    Serial.println("JsonMockContainer!!!: Deconstructor...");
}

void JsonMockContainer::addExpectRequest(const char* request){
    (void)request;
}

void JsonMockContainer::addResponse(const char* response){
    Serial.println("JsonMockContainer!!!: Add response");
    Serial.println(response);
    this->responseList[this->numberResponses] = (char*)response;
    Serial.println(this->responseList[this->numberResponses]);
    this->numberResponses++;
    Serial.println(this->numberResponses);
}

void JsonMockContainer::addEvent(const char* event){
    (void)event;
}

char* JsonMockContainer::getNextResponse(void)
{
    Serial.print("Current: ");
    Serial.print(this->currentResponse);
    Serial.print(" Number: ");
    Serial.println(this->numberResponses);
    if(this->currentResponse >= this->numberResponses) {
        //Serial.println("No more responses in queue");
        return NULL;
    }
    Serial.println("JsonMockContainer!!!: Get Next response");
    this->currentResponse++;
    return this->responseList[this->currentResponse-1];
}

const char* JsonMockContainer::getNextEvent(void)
{
    if(this->currentEvent >= this->numberEvents) {
        Serial.println("JsonMockContainer!!!: No more events in queue");
        return NULL;
    }
    this->currentEvent++;
    return this->eventList[this->currentEvent-1];
}

void JsonMockContainer::clearAll(void)
{
    memset(this->requestList, 0x00, sizeof(responseList));
    memset(this->responseList, 0x00, sizeof(responseList));
    memset(this->eventList, 0x00, sizeof(eventList));

    this->numberRequests = 0;
    this->numberResponses = 0;
    this->numberEvents = 0;
    this->currentRequest = 0;
    this->currentResponse = 0;
    this->currentEvent = 0;
}
