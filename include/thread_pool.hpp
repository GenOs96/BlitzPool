/*
    BlitzPool: Threadpool library
*/
#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <future>
#include <utility>

class ThreadPool{
    private:
        int m_num_threads;
        bool stop;
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> task_queue;
        std::mutex m;
        std::condition_variable cv;
    
    public:
        explicit ThreadPool();

        ~ThreadPool();

        template <class Func, class... Args>
        auto executeTask(Func &&f, Args &&...args) -> std::future<decltype(f(args...))>
        {
            // Get the return type of the task
            using return_type = decltype(f(args...));

            // Extract the task
            auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

            // Get the future return of task's execution
            std::future<return_type> ret = task->get_future();

            std::unique_lock<std::mutex> lock(m);
            task_queue.emplace([task]() -> void
                               { (*task)(); });
            lock.unlock();
            cv.notify_one();
            return ret;
        }
};