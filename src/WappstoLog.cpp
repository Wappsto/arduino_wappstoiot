#include "Arduino.h"
#include "WappstoLog.h"

WappstoLog* WappstoLog::instance()
{
    static WappstoLog instance_;
    return &instance_;
}

void WappstoLog::setLogLevel(LOG_LEVELS_e logLevel)
{
    _logLevel = logLevel;
}

void WappstoLog::error(const char* text)
{
    _print(ERROR, text);
}

void WappstoLog::error(const char* text, int data)
{
    _print(ERROR, text, data);
}

void WappstoLog::error(const char* text, const char* data)
{
    _print(ERROR, text, data);
}

void WappstoLog::error(const JsonDocument& root)
{
    _print(ERROR, root);
}

void WappstoLog::warning(const char* text)
{
    _print(WARNING, text);
}

void WappstoLog::warning(const char* text, int data)
{
    _print(WARNING, text, data);
}

void WappstoLog::warning(const char* text, const char* data)
{
    _print(WARNING, text, data);
}

void WappstoLog::warning(const JsonDocument& root)
{
    _print(WARNING, root);
}

void WappstoLog::info(const char* text)
{
    _print(INFO, text);
}

void WappstoLog::info(const char* text, int data)
{
    _print(INFO, text, data);
}

void WappstoLog::info(const char* text, const char* data)
{
    _print(INFO, text, data);
}

void WappstoLog::info(const JsonDocument& root)
{
    _print(INFO, root);
}

void WappstoLog::verbose(const char* text)
{
    _print(VERBOSE, text);
}

void WappstoLog::verbose(const char* text, int data)
{
    _print(VERBOSE, text, data);
}

void WappstoLog::verbose(const char* text, const char* data)
{
    _print(VERBOSE, text, data);
}

void WappstoLog::verbose(const JsonDocument& root)
{
    _print(VERBOSE, root);
}

bool WappstoLog::shouldPrintLevel(LOG_LEVELS_e logLevel)
{
    if(_logLevel <= logLevel) {
        return true;
    }
    return false;
}

void WappstoLog::_print(LOG_LEVELS_e level, const char* text)
{
    if(shouldPrintLevel(level)) {
        Serial.println(text);
    }
}

void WappstoLog::_print(LOG_LEVELS_e level, const char* text, const char* data)
{
    if(shouldPrintLevel(level)) {
        Serial.print(text);
        Serial.println(data);
    }
}

void WappstoLog::_print(LOG_LEVELS_e level, const char* text, int data)
{
    if(shouldPrintLevel(level)) {
        Serial.print(text);
        Serial.println(data);
    }
}

void WappstoLog::_print(LOG_LEVELS_e level, const JsonDocument& root)
{
    if(shouldPrintLevel(level)) {
        Serial.print("\e[1;31m");
        serializeJsonPretty(root, Serial);
        Serial.println("");
        Serial.print("\e[1;37m");
    }
}