#include <Aoba/AobaCore.hpp>
#include <Aoba/ColorTheme.hpp>
#include <Aoba/WindowManager.hpp>

#include "PageManager/PageManager.hpp"
#include "ParallelTaskManager/ParallelTaskManager.hpp"

void AobaMain();

void Main() {
	(void)s3d::aoba::AobaCore::Instance();
	AobaMain();
}

namespace s3d::aoba {
	void AobaCore::start() {
		if (m_pageManager->initialize()) {
			run();
		}
		else {
			Logger << U"Error(AobaCore): No pages are registered.";
		}
	}

	void AobaCore::run() {
		while (System::Update()) {
			if (System::GetUserActions() == UserAction::CloseButtonClicked) {
				m_pageManager->terminate();
			}

			updateAoba();
		}
	}

	void AobaCore::updateAoba() {
		// Update window state
		WindowManager::Update();

		// Update pages
		m_pageManager->update();

		// Update color theme
		if (m_animateColor) {
			m_animateColor = animateColor();
		}

		// Draw pages, components and events
		m_pageManager->draw();

		// Additional drawing events
		for (auto& f : m_drawingEvents) {
			f();
		}
		m_drawingEvents.release();

		// Update
		updateMainThreadEvents();

		updateTimeouts();
	}

	void AobaCore::updateMainThreadEvents() {
		std::lock_guard<std::mutex> lock(m_mainThreadInserterMutex);

		for (const auto& f : m_eventsRequestedToRunInMainThread) {
			f();
		}

		m_eventsRequestedToRunInMainThread.release();
	}

	void AobaCore::updateTimeouts() {
		bool alive = false;

		for (auto& timeout : m_timeouts) {
			timeout.update();
			alive |= timeout.isAlive();
		}

		if (!alive) {
			m_timeouts.release();
		}
	}

	void AobaCore::terminate() {
		m_pageManager->terminate();
	}
}
