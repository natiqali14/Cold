#include "Clock.h"

u64 Clock::get_current_time_in_us()
{
    auto now  = high_resolution_clock::now();
    return  duration_cast<microseconds> (now - initialise_time).count();
}

u64 Clock::get_current_time_in_ms()
{
    auto now  = high_resolution_clock::now();
    return  duration_cast<milliseconds> (now - initialise_time).count();
}

f32 Clock::get_current_time_in_seconds()
{
    auto now  = high_resolution_clock::now();
    return  duration_cast<seconds> (now - initialise_time).count();
}

Clock::Clock()
: initialise_time(high_resolution_clock::now())
{
}

Clock::~Clock()
{
    
}
