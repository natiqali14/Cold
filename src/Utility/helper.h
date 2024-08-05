
#pragma once
#include "../includes.h"
#include <time.h>
#include <stdio.h>
#include <string>
#include <algorithm>

namespace Cold{
    namespace Helper {
    /// TODO right now this function is only for linux based systems 
        inline i32 sleep_current_thread(u32 seconds, u64 nanoseconds) {
            // struct timespec req, rem;
            // req.tv_sec = seconds;
            // req.tv_nsec = nanoseconds;
            // return nanosleep(&req, &rem);
            return 0;

        }
        inline std::string normalize_paths(const std::string& path) {
            // TODO make this function runnable on both mac / windows platforms
            return path;
            #ifdef win
                std::string normalized_string = path;
                std::replace(normalized_string.begin(), normalized_string.end(), '/', '\\');
                return normalized_string;
            #else return path;
            #endif
        }
    }
}