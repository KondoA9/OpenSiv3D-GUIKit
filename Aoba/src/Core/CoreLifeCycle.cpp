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
	bool Core::Start() {
		if (Instance().m_pageManager->initialize()) {
			Instance().run();
			return true;
		}
		else {
			Logger << U"Aoba(Core) [Error]: No pages are registered.";
			return false;
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
		WindowManager::Update();

		m_pageManager->update();

		if (m_animateColor) {
			m_animateColor = animateColor();
		}

		m_pageManager->draw();

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
