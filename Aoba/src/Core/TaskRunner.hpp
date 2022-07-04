#pragma once

#include "src/Core/TaskRunner/AsyncTaskManager.hpp"
#include "src/Core/TaskRunner/SyncTaskManager.hpp"
#include "src/Core/TaskRunner/TimeoutTaskManager.hpp"

namespace s3d::aoba {
    class TaskRunner final {
    private:
        AsyncTaskManager m_asyncTaskManager;
        SyncTaskManager m_syncTaskManager;
        TimeoutTaskManager m_timeoutTaskManager;

    public:
        bool isAsyncTaskAlive() const noexcept;

        bool isTimeoutTaskAlive(size_t id) const noexcept;

        bool isTimeoutTaskRunning(size_t id) const noexcept;

        void addAsyncTask(const std::function<void()>& task,
                          const std::function<void()>& completion = std::function<void()>());

        void addSyncTask(const std::function<void()>& task);

        size_t addTimeoutTask(const std::function<void()>& task, double ms, bool threading);

        void update();

        bool stopTimeoutTask(size_t id) noexcept;

        bool restartTimeoutTask(size_t id) noexcept;
    };
}
