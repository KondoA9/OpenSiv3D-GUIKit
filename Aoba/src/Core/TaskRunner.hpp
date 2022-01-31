#pragma once

#include <atomic>
#include <functional>

namespace s3d::aoba {
	class TaskRunner final {
	private:
		std::atomic<size_t> m_counter = 0;

	public:
		bool isAlive() const {
			return m_counter != 0;
		}

		void createTask(const std::function<void()>& func, const std::function<void()>& completion = std::function<void()>());
	};
}