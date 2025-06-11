#include "thread_pool.hpp"
#include <iostream>
#include <future>
using namespace std;

ThreadPool::ThreadPool(): m_num_threads(thread::hardware_concurrency()), stop(false){
    cout << "Number of threads: " << m_num_threads << endl;
    for(int idx = 0; idx <  m_num_threads; idx++){
        threads.emplace_back([this](){
            function<void()> task;
            while(1){
                unique_lock<mutex> lock(m);
                cv.wait(lock, [this](){return !task_queue.empty() || stop;});
                
                if(stop && task_queue.empty()) return;

                task = std::move(task_queue.front());
                task_queue.pop();
                lock.unlock();
                task();
            }
        });
    }   
}

ThreadPool::~ThreadPool(){
    unique_lock<mutex> lock(m);
    stop = true;
    lock.unlock();
    cv.notify_all();

    //join all the threads
    for(int idx = 0; idx < m_num_threads; idx++){
        threads[idx].join();
    }

}
