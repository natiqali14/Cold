#pragma once

#include "../../includes.h"
#include <chrono>
using namespace  std::chrono;
class Clock {
public:
u64 get_current_time_in_us();
u64 get_current_time_in_ms();
f32 get_current_time_in_seconds();
Clock();
~Clock();
private:
    time_point<high_resolution_clock> initialise_time;
};