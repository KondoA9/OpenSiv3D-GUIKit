#include <Aoba/Core.hpp>

#include <Aoba/ColorTheme.hpp>

#include "PageManager.hpp"
#include "TaskRunner.hpp"
#include "WindowManager.hpp"

void AobaMain();

void Main() {
	(void)s3d::aoba::Core::Instance();
	AobaMain();
}

namespace s3d::aoba {
	void Core::Start() {
		if (Instance().m_pageManager->initialize()) {
			Instance().run();
		}
		else {
			Logger << U"Error(Core): No pages are registered.";
		}
	}

	void Core::Terminate() {
		Instance().m_pageManager->terminate();
	}

	void Core::run() {
		while (System::Update()) {
			if (System::GetUserActions() == UserAction::CloseButtonClicked) {
				m_pageManager->terminate();
			}

			update();
		}
	}

	void Core::update() {
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

		// Update
		m_taskRunner->runSyncTasks();

		updateTimeouts();
	}

	void Core::updateTimeouts() {
		bool alive = false;

		for (auto& timeout : m_timeouts) {
			timeout.update();
			alive |= timeout.isAlive();
		}

		if (!alive) {
			m_timeouts.release();
		}
	}
}
