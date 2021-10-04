#include <GUIKit/Timeout.hpp>

#include <thread>
#include <atomic>

namespace s3d::gui {
	std::atomic<size_t> Timeout::m_Id = 0;

	Timeout::Timeout(const std::function<void()>& func, double ms, bool threading) :
		m_id(m_Id),
		m_ms(ms),
		m_threading(threading),
		m_func(func)
	{
		m_Id++;
		m_stopwatch.start();
	}

	bool Timeout::isRunning() {
		return m_started && !m_finished;
	}

	bool Timeout::isAlive() {
		return !m_stopped && !m_finished;
	}

	void Timeout::update() {
		if (!m_started && !m_stopped && m_stopwatch.ms() > m_ms) {
			m_started = true;
			if (m_threading) {
				std::thread thread([this] {
					m_func();
					m_finished = true;
					});

				thread.detach();
			}
			else {
				m_func();
				m_finished = true;
			}
		}
	}

	bool Timeout::restart() {
		if (isAlive() && !isRunning()) {
			m_stopwatch.restart();
			return true;
		}
		return false;
	}

	bool Timeout::stop() {
		if (isAlive() && !isRunning()) {
			m_stopped = true;
			return true;
		}
		return false;
	}
}
