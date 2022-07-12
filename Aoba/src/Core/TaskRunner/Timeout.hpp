#pragma once

#include <atomic>
#include <chrono>
#include <functional>

namespace s3d::aoba {
    class Timeout {
        static std::atomic<size_t> m_Id;

        const size_t m_id;
        const uint64_t m_ms;
        const bool m_threading;
        const std::function<void()> m_func;

        std::chrono::system_clock::time_point m_startedTime;

        bool m_started = false, m_stopped = false, m_finished = false;

    public:
        Timeout(const std::function<void()>& func, uint64_t ms, bool threading);

        Timeout(const Timeout&) = delete;

        Timeout(Timeout&&) = default;

        size_t id() const noexcept {
            return m_id;
        }

        bool isPending() const noexcept {
            return !m_started && !m_stopped;
        }

        bool isRunning() const noexcept {
            return m_started && !m_finished;
        }

        bool isAlive() const noexcept {
            return !m_stopped && !m_finished;
        }

        void update();

        bool restart() noexcept;

        bool stop() noexcept;

        Timeout& operator=(const Timeout&) = delete;

        Timeout& operator=(Timeout&&) = delete;
    };
}
