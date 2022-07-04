#pragma once

#include <atomic>
#include <functional>

namespace s3d::aoba {
    class AsyncTaskManager final {
    private:
        std::atomic<size_t> m_taskCount = 0;

    public:
        bool isAlive() const noexcept {
            return m_taskCount != 0;
        }

        void addTask(const std::function<void()>& task, const std::function<void()>& completion);
    };
}
