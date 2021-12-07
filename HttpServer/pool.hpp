#pragma once

#include <queue>
#include <vector>
#include <utility>
#include <atomic>
#include <thread>
#include <future>
#include <condition_variable>
#include <functional>
#include <cstdio>

class ThreadPool {

private:
    struct PoolData {
        using TaskType = std::function<void()>;
        std::queue<TaskType> taskQueue;
        std::vector<std::thread> pool;
        std::mutex myLock;
        std::condition_variable taskCondVar;
        std::atomic_bool isStop = false;
    };
    std::shared_ptr<PoolData> dataPtr;

public:
    ThreadPool(size_t ThreadNum)
        : dataPtr(std::make_shared<PoolData>())
    {
        for (size_t i = 0; i < ThreadNum; ++i) {
            auto& ptr = dataPtr;
            dataPtr->pool.emplace_back([ptr, i]() {
                std::unique_lock<std::mutex> lock(ptr->myLock);
                while (1) {
                    if (ptr->taskQueue.size()) {
                        auto currentThread = std::move(ptr->taskQueue.front());
                        ptr->taskQueue.pop();
                        lock.unlock();

                        // printf("Thread %zuth running\n", i);
                        currentThread();
                        // printf("Thread %zuth finish\n", i);

                        lock.lock();
                    } else if (ptr->isStop.load()) {
                        break;
                    } else {
                        ptr->taskCondVar.wait(lock);
                    }
                }
            });
        }
    }

    ~ThreadPool()
    {
        dataPtr->isStop.store(true);
        dataPtr->taskCondVar.notify_all();
        for (auto& thread : dataPtr->pool) {
            thread.join();
        }
    }

    template <class F>
    void CommitVoidFunction(F&& f)
    {
        {
            std::lock_guard<std::mutex> lock(dataPtr->myLock);
            dataPtr->taskQueue.emplace(std::forward<F>(f));
        }
        dataPtr->taskCondVar.notify_one();
    }

    // all of the function should be copyable.
    template <class F, class...Arg>
    auto CommitNormalFunction(F&& f, Arg&&... args)
        -> std::future<decltype(f(args...))>
    {
        using ReturnType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Arg>(args)...));
        std::future<ReturnType> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(dataPtr->myLock);
            dataPtr->taskQueue.emplace([task]() { (*task)(); });
        }

        dataPtr->taskCondVar.notify_one();
        return res;
    }
};