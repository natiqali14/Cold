//
// Created by Natiq on 12/7/2024.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <includes.h>
#include <mutex>
#include <thread>
#include <future>
#include <vector>
#include "ThreadSafeQueue.h"
#include "Worker.h"
namespace Cold {
    struct ThreadWorker {
        std::thread worker_thread;
        std::shared_ptr<Worker> worker;
        ThreadWorker() {
            worker = std::make_shared<Worker>();
            worker_thread = std::thread([this]() {
               worker->run();
            });
        }
        ~ThreadWorker() {
            worker->force_wake_up_queue();
            if (worker_thread.joinable()) worker_thread.join();
        }

    };
    class ThreadPool {
    public:
        ThreadPool(u8 number_of_threads = 5);
        ~ThreadPool();
        static void start(u8 number_of_threads);
        static void stop();
        static ThreadPool* get_instance();

        template<typename Func, typename... Args>
        auto submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func,  Args...>>
        {
            using ReturnType = std::invoke_result_t<Func,  Args...>;
            // 1. making packaged task
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
                );
            //2. add to workers
            auto job_worker = get_available_worker();
            job_worker->add_task([task]() {
                (*task)();
            });
            // 3. returning future for task result
            std::future<ReturnType> r = task->get_future();
            return r;
        }

    private:
        inline static ThreadPool* instance = nullptr;
        u8 total_number_of_threads;
        std::vector<std::shared_ptr<ThreadWorker>> job_workers;

        // functions
        std::shared_ptr<Worker> get_available_worker();
    };
}




#endif //THREADPOOL_H
