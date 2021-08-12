#include "ParallelTaskManager.hpp"

#include <thread>

namespace s3d::gui {
	void ParallelTaskManager::createTask(const std::function<void()>& func, const std::function<void()>& completion) {
		m_counter++;

		std::thread thread([this, func, completion]() {
			func();

			if (completion) {
				completion();
			}

			m_counter--;
			});

		thread.detach();
	}
}
