#include "MockUTCTime.h"

static char strfTimeBuffer[21] = "";

const char* getUtcTime(void)
{
    return strfTimeBuffer;
}
