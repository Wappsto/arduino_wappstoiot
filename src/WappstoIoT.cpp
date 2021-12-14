#include "Arduino.h"
#include "WappstoIoT.h"

void generateNewUuid(char *str)
{
    sprintf(str, "%04x%04x-%04x-%04x-%04x-%04x%04x%04x",
            random(0xFFFF) & 0xFFFF,
            random(0xFFFF) & 0xFFFF,
            random(0xFFFF) & 0xFFFF,
            random(0xFFFF) & 0xFFFF,
            random(0xFFFF) & 0xBFFF, // UUID variant = 8 .. b
            random(0xFFFF) & 0xFFFF,
            random(0xFFFF) & 0xFFFF,
            random(0xFFFF) & 0xFFFF);
    // UUID verion 4 = random
    str[14] = '4';
    str[36] = '\0';
}
