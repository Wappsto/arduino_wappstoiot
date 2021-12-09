
#include "WappstoIoT.h"

void generateNewUuid(char *str)
{
#if defined(ESP32)
    sprintf(str, "%08x-%04x-%04x-%04x-%04x%08x",
            esp_random(),
            esp_random() & 0xFFFF,
            esp_random() & 0xFFFF,
            esp_random() & 0xBFFF, // UUID variant = 8 .. b
            esp_random() & 0xFFFF,
            esp_random());
#else
    sprintf(str, "%08x-%04x-%04x-%04x-%04x%08x",
            random(),
            random() & 0xFFFF,
            random() & 0xFFFF,
            random() & 0xBFFF, // UUID variant = 8 .. b
            random() & 0xFFFF,
            random());
#endif
    // UUID verion 4 = random
    str[14] = '4';
    str[36] = '\0';
}
