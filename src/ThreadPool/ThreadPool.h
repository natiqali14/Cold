//
// Created by Natiq on 12/7/2024.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <includes.h>
#include <mutex>
#include <thread>
#include <future>
#include "ThreadSafeQueue.h"
namespace Cold {
    class ThreadPool {
    public:
        ThreadPool(u8 number_of_threads);
        void start();
        void stop();

        template<typename Func, typename... Args>
        auto submit(Func&& func, Args&&... args) -> std::future<decltype(std::invoke(std::forward<Func>(func), std::forward<Args>(args)))>
        {

        }

    private:
        u8 total_number_of_threads;
    };
}




#endif //THREADPOOL_H
