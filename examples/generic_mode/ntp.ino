#include "time.h"

const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;
const char* ntpServer = "pool.ntp.org";
static char strfTimeBuffer[21];

void initializeNtp(void)
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // required for timestamp
}

const char* getUtcTime(void)
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
    }
    strftime(strfTimeBuffer, sizeof(strfTimeBuffer), "%FT%TZ", &timeinfo);
    return strfTimeBuffer;
}
