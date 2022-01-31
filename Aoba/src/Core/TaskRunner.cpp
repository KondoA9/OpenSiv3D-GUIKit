#include "TaskRunner.hpp"

#include <thread>

namespace s3d::aoba {
	void TaskRunner::createTask(const std::function<void()>& func, const std::function<void()>& completion) {
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
