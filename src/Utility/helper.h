
#pragma once
#include "../includes.h"
#include <time.h>
#include <stdio.h>
namespace Engine{
    namespace Helper {
    /// TODO right now this function is only for linux based systems 
        i32 sleep_current_thread(u32 seconds, u64 nanoseconds) {
            struct timespec req, rem;
            req.tv_sec = seconds;
            req.tv_nsec = nanoseconds;
            return nanosleep(&req, &rem);

        }
    }
}