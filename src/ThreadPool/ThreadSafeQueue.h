//
// Created by Natiq on 12/7/2024.
//

#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>
#include <includes.h>
namespace Cold {
    template<typename T>
    class ThreadSafeQueue {
        using Type = T;
        std::deque<Type> queue;
        mutable std::mutex q_mtx;
        std::condition_variable q_cond;
        bool force {false};
    public:
        ThreadSafeQueue() = default;
        ~ThreadSafeQueue() = default;

        void force_wake_up(bool flag) {
            std::lock_guard<std::mutex> lock(q_mtx);
            force = true;
            q_cond.notify_all();

        }
        void push(Type element) {
            std::lock_guard<std::mutex> lock(q_mtx);
            queue.push_back(element);
            q_cond.notify_all();
        }

        i32 size() const {
            std::lock_guard<std::mutex> lock(q_mtx);
            return queue.size();
        }

        Type& wait_and_pop() {
            std::lock_guard<std::mutex> lock(q_mtx);
            q_cond.wait(lock, [this](){return !queue.empty();});
            Type element = std::move(queue.front());
            queue.pop_front();
            return element;
        }

        void wait_and_pop(std::shared_ptr<Type>& element) {
            std::unique_lock<std::mutex> lock(q_mtx);
            q_cond.wait(lock, [this](){return !queue.empty() || force;});
            if (force) {
                element = nullptr;
            }
            element = std::make_shared<T>(std::move(queue.front()));
            queue.pop_front();
        }

        void wait_and_pop(Type& element) {
            std::lock_guard<std::mutex> lock(q_mtx);
            q_cond.wait(lock, [this](){return !queue.empty();});
            element = std::move(queue.front());
            queue.pop_front();
        }

        std::shared_ptr<Type> try_and_pop() {
            std::lock_guard<std::mutex> lock(q_mtx);
            if (queue.empty()) {
                return nullptr;
            }
            auto element = std::make_shared<Type>(std::move(queue.front()));
            queue.pop_front();
            return element;
        }

        bool try_and_pop(Type& element) {
            std::lock_guard<std::mutex> lock(q_mtx);
            if (queue.empty()) return false;
            element = std::move(queue.front());
            queue.pop_front();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(q_mtx);
            return queue.empty();
        }


    };
}
#endif //THREADSAFEQUEUE_H
