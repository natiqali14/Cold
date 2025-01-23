//
// Created by frio on 14/01/2025.
//

#ifndef WORKER_H
#define WORKER_H


#include <includes.h>
#include "ThreadSafeQueue.h"
#include <functional>
namespace Cold {
    class Worker {
        using VoidFtn = std::function<void()>;
    public:
        ~Worker();
        void run();
        void add_task(VoidFtn task);
        std::shared_ptr<VoidFtn> get_next_task_to_run();
        u64 get_total_number_of_tasks();
        void force_wake_up_queue();
    private:
        ThreadSafeQueue<VoidFtn> tasks;
        std::atomic<bool> b_stop_worker {false};
    };
}
#endif //WORKER_H
