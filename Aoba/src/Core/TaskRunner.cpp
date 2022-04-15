#include "TaskRunner.hpp"

#include <thread>

namespace s3d::aoba {
    void TaskRunner::AsyncTaskManager::addTask(const std::function<void()>& task,
                                               const std::function<void()>& completion) {
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

    void TaskRunner::SyncTaskManager::addTask(const std::function<void()>& task) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.emplace_back(task);
    }

    void TaskRunner::SyncTaskManager::run() {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (const auto& task : m_tasks) {
            task();
        }

        // release tasks
        m_tasks.clear();
        m_tasks.shrink_to_fit();
    }
}
