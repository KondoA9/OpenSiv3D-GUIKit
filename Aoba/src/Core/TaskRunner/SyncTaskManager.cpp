#include "SyncTaskManager.hpp"

namespace s3d::aoba {
    void SyncTaskManager::addTask(const std::function<void()>& task) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.emplace_back(task);
    }

    void SyncTaskManager::run() {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (const auto& task : m_tasks) {
            task();
        }

        // release tasks
        m_tasks.clear();
        m_tasks.shrink_to_fit();
    }
}
