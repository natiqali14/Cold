//
// Created by frio on 14/01/2025.
//

#include "Worker.h"

#include <Logger.h>

namespace Cold {
    Worker::~Worker() {

    }

    void Worker::run() {
        while (auto task = tasks.wait_and_pop_with_ret()) {
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
        b_stop_worker = true;
        tasks.force_wake_up(true);
    }
}

