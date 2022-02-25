#include "TaskRunner.hpp"

#include <thread>

namespace s3d::aoba {
    void TaskRunner::AsyncTaskManager::addTask(const std::function<void()>& task,
                                               const std::function<void()>& completion) {
        m_counter++;

        std::thread thread([this, task, completion]() {
            task();

            if (completion) {
                completion();
            }

            m_counter--;
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

        m_tasks.release();
    }
}
