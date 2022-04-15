#pragma once

#include <functional>
#include <mutex>
#include <vector>

namespace s3d::aoba {
    class TaskRunner final {
        class AsyncTaskManager final {
        private:
            std::atomic<size_t> m_taskCount = 0;

        public:
            bool isAlive() const {
                return m_taskCount != 0;
            }

            void addTask(const std::function<void()>& task, const std::function<void()>& completion);
        };

        class SyncTaskManager final {
        private:
            std::mutex m_mutex;
            std::vector<std::function<void()>> m_tasks;

        public:
            void addTask(const std::function<void()>& task);

            void run();
        };

    private:
        AsyncTaskManager m_asyncTaskManager;
        SyncTaskManager m_syncTaskManager;

    public:
        bool isAsyncTaskAlive() const {
            return m_asyncTaskManager.isAlive();
        }

        void addAsyncTask(const std::function<void()>& task,
                          const std::function<void()>& completion = std::function<void()>()) {
            m_asyncTaskManager.addTask(task, completion);
        }

        void addSyncTask(const std::function<void()>& task) {
            m_syncTaskManager.addTask(task);
        }

        void runSyncTasks() {
            m_syncTaskManager.run();
        }
    };
}
