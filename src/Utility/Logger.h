#pragma once
#include "../includes.h"
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


#define ERROR(msg, ...) Cold::Logger::log(Cold::Logger::LogType::LOGTYPE_ERROR, msg, ##__VA_ARGS__)
#define WARNING(msg, ...) Cold::Logger::log(Cold::Logger::LogType::LOGTYPE_WARNING, msg, ##__VA_ARGS__)

#if DEBUG
#define INFO(msg, ...) Cold::Logger::log(Cold::Logger::LogType::LOGTYPE_INFO, msg, ##__VA_ARGS__)
#define TRACE(msg, ...) Cold::Logger::log(Cold::Logger::LogType::LOGTYPE_TRACE, msg, ##__VA_ARGS__)
#else

#define INFO(msg, ...)
#define TRACE(msg, ...
#endif)