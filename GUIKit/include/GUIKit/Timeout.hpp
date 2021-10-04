#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	class Timeout {
		static std::atomic<size_t> m_Id;

		const size_t m_id;
		const double m_ms;
		const bool m_threading;
		const std::function<void()> m_func;

		Stopwatch m_stopwatch;
		bool m_started = false, m_stopped = false, m_finished = false;

	public:
		Timeout(const std::function<void()>& func, double ms, bool threading);

		Timeout(const Timeout&) = delete;

		Timeout(Timeout&&) = default;

		size_t id() const {
			return m_id;
		}

		bool isRunning();

		bool isAlive();

		void update();

		bool restart();

		bool stop();

		Timeout& operator=(const Timeout&) = delete;

		Timeout& operator=(Timeout&&) = delete;
	};
}
