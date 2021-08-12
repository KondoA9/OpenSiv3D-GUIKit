#pragma once

#include <functional>
#include <atomic>

namespace s3d::gui {
	class AsyncProcessManager final {
	private:
		std::atomic<size_t> m_counter = 0;

	public:
		bool isAlive() const {
			return m_counter == 0;
		}

		void create(const std::function<void()>& func, const std::function<void()>& completion = std::function<void()>());
	};
}