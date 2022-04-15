#pragma once

#include <functional>
#include <vector>

#include "src/Core/TaskRunner/Timeout.hpp"

namespace s3d::aoba {
    class TimeoutTaskManager final {
    private:
        std::vector<Timeout> m_timeouts;

    public:
        bool isAlive(size_t id) const;

        bool isRunning(size_t id) const;

        size_t addTask(const std::function<void()>& task, double ms, bool threading);

        void update();

        bool stop(size_t id);

        bool restart(size_t id);
    };
}
