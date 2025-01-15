//
// Created by frio on 15/01/2025.
//

#ifndef GLCOMMANDCENTRE_H
#define GLCOMMANDCENTRE_H

#include <future>
#include <functional>
#include "ThreadPool/ThreadSafeQueue.h"

namespace Cold {
    class GLCommandCentre {
    public:
        static void start();
        static void stop();
        static GLCommandCentre* get_instance();

        template<typename Func, typename... Args>
        auto submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func,  Args...>>
        {
            using ReturnType = std::invoke_result_t<Func,  Args...>;
            // 1. making packaged task
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
                );
            //2. add to Q

            gl_queue.push([task]() {
                (*task)();
            });
            // 3. returning future for task result
            std::future<ReturnType> r = task->get_future();
            return r;
        }

        void run_commands();
    private:
        inline static GLCommandCentre* instance = nullptr;
        ThreadSafeQueue<std::function<void()>> gl_queue;
    };

}



#endif //GLCOMMANDCENTRE_H
