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
        auto submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func,  Args...>>
        {
            using ReturnType = std::future<std::invoke_result_t<Func,  Args...>>;
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
                );
            std::future<ReturnType> r = task->get_future();
            return r;
        }

    private:
        u8 total_number_of_threads;
    };
}




#endif //THREADPOOL_H
