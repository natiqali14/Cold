//
// Created by Natiq on 12/7/2024.
//

#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>
namespace Cold {
    template<typename T>
    class ThreadSafeQueue {
        using Type = T;
        std::deque<Type> queue;
        mutable std::mutex q_mtx;
        std::condition_variable q_cond;
    public:
        ThreadSafeQueue() = default;
        ~ThreadSafeQueue() = default;

        void push(Type element) {
            std::lock_guard<std::mutex> lock(q_mtx);
            queue.push_back(element);
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
