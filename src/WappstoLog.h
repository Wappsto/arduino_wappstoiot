#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#define JSON_POST_BUFFER 2072

typedef enum {
   VERBOSE,
   INFO,
   WARNING,
   ERROR,
   NO_LOGS,
} LOG_LEVELS_e;


class WappstoLog
{
    public:
        static WappstoLog* instance();
        void setLogLevel(LOG_LEVELS_e logLevel);
        void error(const char* text);
        void error(const char* text, int data);
        void error(const char* text, const char* data);
        void error(const JsonDocument& root);
        void warning(const char* text);
        void warning(const char* text, int data);
        void warning(const char* text, const char* data);
        void warning(const JsonDocument& root);
        void info(const char* text);
        void info(const char* text, int data);
        void info(const char* text, const char* data);
        void info(const JsonDocument& root);
        void verbose(const char* text);
        void verbose(const char* text, int data);
        void verbose(const char* text, const char* data);
        void verbose(const JsonDocument& root);
        bool shouldPrintLevel(LOG_LEVELS_e logLevel);


    private:
        WappstoLog() = default;
        ~WappstoLog() {};
        WappstoLog(const WappstoLog&) = delete;
        WappstoLog& operator=(const WappstoLog&) = delete;

        LOG_LEVELS_e _logLevel;
        void _print(const char* text);
        void _print(const char* text, const char* data);
        void _print(const char* text, int data);
};
