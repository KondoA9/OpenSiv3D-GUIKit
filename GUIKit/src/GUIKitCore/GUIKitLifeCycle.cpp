#include <GUIKit/GUIKitCore.hpp>
#include <GUIKit/WindowManager.hpp>
#include <GUIKit/UnifiedFont.hpp>

#include "PageManager/PageManager.hpp"
#include "ParallelTaskManager/ParallelTaskManager.hpp"

namespace s3d::gui {
	GUIKitCore::GUIKitCore() {
		m_pageManager = new PageManager();
		m_parallelTaskManager = new ParallelTaskManager();

		UnifiedFont::Initialize();

		WindowManager::Initialize();

		Scene::SetResizeMode(ResizeMode::Actual);

		System::SetTerminationTriggers(UserAction::NoAction);
	}

	void GUIKitCore::start() {
		if (m_pageManager->initialize()) {
			run();
		}
		else {
			Logger << U"Error(GUIKitCore): No pages are registered.";
		}

		delete m_pageManager;
		delete m_parallelTaskManager;
	}

	void GUIKitCore::run() {
		while (System::Update()) {
			if (System::GetUserActions() == UserAction::CloseButtonClicked) {
				m_pageManager->terminate();
			}

			updateGUIKit();
		}
	}

	void GUIKitCore::updateGUIKit() {
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

	void GUIKitCore::updateMainThreadEvents() {
		std::lock_guard<std::mutex> lock(m_mainThreadInserterMutex);

		for (const auto& f : m_eventsRequestedToRunInMainThread) {
			f();
		}

		m_eventsRequestedToRunInMainThread.release();
	}

	void GUIKitCore::updateTimeouts() {
		bool alive = false;

		for (auto& timeout : m_timeouts) {
			timeout.update();
			alive |= timeout.isAlive();
		}

		if (!alive) {
			m_timeouts.release();
		}
	}

	void GUIKitCore::terminate() {
		m_pageManager->terminate();
	}
}
