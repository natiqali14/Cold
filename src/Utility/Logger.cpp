#include "Logger.h"
#include <sstream>
#include <stdio.h>
#include <stdarg.h>
namespace Cold {
    namespace Logger {
        static std::string get_log_type(Cold::Logger::LogType type) {
            switch (type) {
                case LogType::LOGTYPE_TRACE:
                    return "[TRACE]: ";
                case LogType::LOGTYPE_ERROR:
                    return "[ERROR]: ";
                case LogType::LOGTYPE_WARNING:
                    return "[WARNING]: ";
                case LogType::LOGTYPE_INFO:
                    return "[INFO]: ";
            }
            return "[DEFAULT]";
        }
            

        void log(LogType type, const char*  msg, ...)
        {
            char message[4096 + 1];
            va_list args;
            va_start(args, msg);
            vsnprintf(message, sizeof(message), msg, args);
            va_end(args);
            std::cout << Cold::Logger::get_code(type) << get_log_type(type) << message << "\n";

        }
        std::string get_code(LogType type)
        {
            std::stringstream ss;
            ss << "\033[" << static_cast<i32>(type) << "m";
            return ss.str();
}
    }
}
