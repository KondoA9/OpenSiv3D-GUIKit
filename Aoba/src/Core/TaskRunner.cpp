#include "TaskRunner.hpp"

namespace s3d::aoba {
    bool TaskRunner::isAsyncTaskAlive() const {
        return m_asyncTaskManager.isAlive();
    }

    bool TaskRunner::isTimeoutTaskAlive(size_t id) const {
        return m_timeoutTaskManager.isAlive(id);
    }

    bool TaskRunner::isTimeoutTaskRunning(size_t id) const {
        return m_timeoutTaskManager.isRunning(id);
    }

    void TaskRunner::addAsyncTask(const std::function<void()>& task, const std::function<void()>& completion) {
        m_asyncTaskManager.addTask(task, completion);
    }

    void TaskRunner::addSyncTask(const std::function<void()>& task) {
        m_syncTaskManager.addTask(task);
    }

    size_t TaskRunner::addTimeoutTask(const std::function<void()>& task, double ms, bool threading) {
        return m_timeoutTaskManager.addTask(task, ms, threading);
    }

    void TaskRunner::update() {
        m_syncTaskManager.run();
        m_timeoutTaskManager.update();
    }

    bool TaskRunner::stopTimeoutTask(size_t id) {
        return m_timeoutTaskManager.stop(id);
    }

    bool TaskRunner::restartTimeoutTask(size_t id) {
        return m_timeoutTaskManager.restart(id);
    }
}
