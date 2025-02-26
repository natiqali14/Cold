//
// Created by Natiq on 12/7/2024.
//

#include "ThreadPool.h"

namespace Cold {
    ThreadPool::ThreadPool(u8 number_of_threads): total_number_of_threads(number_of_threads) {
        for (u8 i = 0; i < total_number_of_threads; i++) {
            auto worker = std::make_shared<ThreadWorker>();
            job_workers.push_back(worker);
        }
    }

    ThreadPool::~ThreadPool() {
        COLD_TRACE("ThreadPool::~ThreadPool Called");
    }

    void ThreadPool::start(u8 number_of_threads) {
        instance = new ThreadPool(number_of_threads);
    }

    void ThreadPool::stop() {
        delete instance;
        instance = nullptr;
    }

    ThreadPool * ThreadPool::get_instance() {
        return instance;
    }

    std::shared_ptr<Worker> ThreadPool::get_available_worker() {
        u64 total_tasks = 0;
        std::shared_ptr<Worker> ret;
        for (auto w : job_workers) {
            if (w->worker->get_total_number_of_tasks() == 0) {
                return w->worker;
            }
            if (total_tasks > w->worker->get_total_number_of_tasks()) {
                ret = w->worker;
                total_tasks = ret->get_total_number_of_tasks();
            }
        }
        return ret;
    }
}
