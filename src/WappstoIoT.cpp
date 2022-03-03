#include "Arduino.h"
#include "WappstoIoT.h"
#include "WappstoRpc.h"

void generateNewUuid(char *str)
{
    sprintf(str, "%04x%04x-%04x-%04x-%04x-%04x%04x%04x",
            (int)(random(0xFFFF) & 0xFFFF),
            (int)(random(0xFFFF) & 0xFFFF),
            (int)(random(0xFFFF) & 0xFFFF),
            (int)(random(0xFFFF) & 0xFFFF),
            (int)(random(0xFFFF) & 0xBFFF), // UUID variant = 8 .. b
            (int)(random(0xFFFF) & 0xFFFF),
            (int)(random(0xFFFF) & 0xFFFF),
            (int)(random(0xFFFF) & 0xFFFF));
    // UUID verion 4 = random
    str[14] = '4';
    str[36] = '\0';
}
