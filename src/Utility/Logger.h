#pragma once
#include "../includes.h"
#include <csignal>
#include <string>
#include <stdlib.h>
namespace Cold {
    namespace Logger {

        enum class LogType : u8 {
            LOGTYPE_TRACE = 0,
            LOGTYPE_ERROR = 31,
            LOGTYPE_INFO = 32,
            LOGTYPE_WARNING = 33,
        
        };
        void log(LogType type, const char*  msg, ...);
        std::string get_code(LogType type);
    }
}

#define TRACE_ENABLED 0
#define ERROR_ENABLED 1
#define INFO_ENABLED 0
#define WARNING_ENABLED 1

#define COLD_ERROR(msg, ...) Cold::Logger::log(Cold::Logger::LogType::LOGTYPE_ERROR, msg, ##__VA_ARGS__)
#define COLD_WARNING(msg, ...) Cold::Logger::log(Cold::Logger::LogType::LOGTYPE_WARNING, msg, ##__VA_ARGS__)

#define COLD_ASSERT(expression, msg) {                                                                            \
    if(expression) {}                                                                                        \
    else {                                                                                                   \
        COLD_ERROR("-ASSERTION FAILED- expression %s, with msg %s at line %d at file %s", #expression, msg, __LINE__, __FILE__);  \
        abort();                                                                                \
    }                                                                                                        \
}

#if DEBUG
    #if INFO_ENABLED
        #define COLD_INFO(msg,...) Cold::Logger::log(Cold::Logger::LogType::LOGTYPE_INFO, msg, ##__VA_ARGS__)
    #else
        #define COLD_INFO(msg, ...)
    #endif

    #if TRACE_ENABLED
        #define COLD_TRACE(msg, ...) Cold::Logger::log(Cold::Logger::LogType::LOGTYPE_TRACE, msg, ##__VA_ARGS__)
    #else
        #define COLD_TRACE(msg, ...)
    #endif
#else

#define COLD_INFO(msg, ...)
#define COLD_TRACE(msg, ...)
#endif