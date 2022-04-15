#include "AsyncTaskManager.hpp"

#include <thread>

namespace s3d::aoba {
    void AsyncTaskManager::addTask(const std::function<void()>& task, const std::function<void()>& completion) {
        m_taskCount++;

        std::thread thread([this, task, completion]() {
            task();

            if (completion) {
                completion();
            }

            m_taskCount--;
        });

        thread.detach();
    }
}
