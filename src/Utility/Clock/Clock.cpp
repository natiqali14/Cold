#include "Clock.h"
namespace Cold {
    u64 Clock::get_current_time_in_us()
    {
        auto now  = std::chrono::high_resolution_clock::now();
        return  std::chrono::duration_cast<std::chrono::microseconds> (now - initialise_time).count();
    }

    u64 Clock::get_current_time_in_ms()
    {
        auto now  = std::chrono::high_resolution_clock::now();
        return  std::chrono::duration_cast<std::chrono::milliseconds> (now - initialise_time).count();
    }

    f32 Clock::get_current_time_in_seconds()
    {
        auto now  = std::chrono::high_resolution_clock::now();
        return  std::chrono::duration_cast<std::chrono::seconds> (now - initialise_time).count();
    }

    Clock::Clock()
    : initialise_time(std::chrono::high_resolution_clock::now())
    {
    }

    Clock::~Clock()
    {
        
    }

}
