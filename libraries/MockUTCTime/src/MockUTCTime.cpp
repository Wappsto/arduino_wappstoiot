#include "MockUTCTime.h"

static char strfTimeBuffer[21] = "2022-02-16T09:30:18Z";

const char* getUtcTime(void)
{
    return strfTimeBuffer;
}
