//
// Created by frio on 14/01/2025.
//

#include "Worker.h"

#include <Logger.h>

namespace Cold {
    void Worker::run() {
        while (!b_stop_worker) {
            std::shared_ptr<VoidFtn> task;
            tasks.wait_and_pop(task);
            if (!task) break;
            (*task)();
        }
    }

    void Worker::add_task(VoidFtn task) {
        tasks.push(task);
    }

    std::shared_ptr<std::function<void()>> Worker::get_next_task_to_run() {
        auto task = tasks.try_and_pop();
        return task;
    }

    u64 Worker::get_total_number_of_tasks() {
        return tasks.size();
    }

    void Worker::force_wake_up_queue() {
        tasks.force_wake_up(true);
    }
}

