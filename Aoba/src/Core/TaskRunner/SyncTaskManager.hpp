#pragma once

#include <functional>
#include <mutex>
#include <vector>

namespace s3d::aoba {
    class SyncTaskManager final {
    private:
        std::mutex m_mutex;
        std::vector<std::function<void()>> m_tasks;

    public:
        void addTask(const std::function<void()>& task);

        void run();
    };
}
