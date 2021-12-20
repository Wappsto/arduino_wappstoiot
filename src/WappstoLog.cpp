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
    if(_logLevel <= ERROR) {
        _print(text);
    }
}
void WappstoLog::error(const char* text, int data)
{
    if(_logLevel <= ERROR) {
        _print(text, data);
    }
}
void WappstoLog::error(const char* text, const char* data)
{
    if(_logLevel <= ERROR) {
        _print(text, data);
    }
}
void WappstoLog::warning(const char* text)
{
    if(_logLevel <= WARNING) {
        _print(text);
    }
}
void WappstoLog::warning(const char* text, int data)
{
    if(_logLevel <= WARNING) {
        _print(text, data);
    }
}
void WappstoLog::warning(const char* text, const char* data)
{
    if(_logLevel <= WARNING) {
        _print(text, data);
    }
}
void WappstoLog::info(const char* text)
{
    if(_logLevel <= INFO) {
        _print(text);
    }
}
void WappstoLog::info(const char* text, int data)
{
    if(_logLevel <= INFO) {
        _print(text, data);
    }
}
void WappstoLog::info(const char* text, const char* data)
{
    if(_logLevel <= INFO) {
        _print(text, data);
    }
}
void WappstoLog::verbose(const char* text)
{
    if(_logLevel <= VERBOSE) {
        _print(text);
    }
}
void WappstoLog::verbose(const char* text, int data)
{
    if(_logLevel <= VERBOSE) {
        _print(text, data);
    }
}
void WappstoLog::verbose(const char* text, const char* data)
{
    if(_logLevel <= VERBOSE) {
        _print(text, data);
    }
}

bool WappstoLog::shouldPrintLevel(LOG_LEVELS_e logLevel)
{
    if(_logLevel <= logLevel) {
        return true;
    }
    return false;
}

void WappstoLog::_print(const char* text)
{
    Serial.println(text);
}

void WappstoLog::_print(const char* text, const char* data)
{
    Serial.print(text);
    Serial.println(data);
}

void WappstoLog::_print(const char* text, int data)
{
    Serial.print(text);
    Serial.println(data);
}
