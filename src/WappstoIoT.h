#pragma once

#define UUID_LENGTH 37
#define TIMESTAMP_LENGTH 30

typedef enum {
    REQUEST_GET,
    REQUEST_PUT,
    REQUEST_DELETE,
    REQUEST_SUCCESS,
    REQUEST_UNKNOWN,
} RequestType_e;

typedef enum {
    TYPE_REPORT,
    TYPE_CONTROL,
} StateType_e;

typedef char UUID_t[UUID_LENGTH];
typedef char Timestamp_t[TIMESTAMP_LENGTH];

void generateNewUuid(char* str);
const char* getUtcTime(void);
