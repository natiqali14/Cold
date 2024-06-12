#include "Logger.h"
#include <sstream>
void Cold::Logger::log(LogType type, const char*  msg, ...)
{
    char message[4096 + 1];
    va_list args;
    va_start(args, msg);
    vsnprintf(message, sizeof(message), msg, args);
    va_end(args);
    std::cout << Cold::Logger::get_code(type) << message << "\n";

}
std::string Cold::Logger::get_code(LogType type)
{
    std::stringstream ss;
    ss << "\033[" << static_cast<i32>(type) << "m";
    return ss.str();
}
