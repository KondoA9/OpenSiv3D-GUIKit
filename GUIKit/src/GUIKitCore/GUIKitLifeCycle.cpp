#include <GUIKit/GUIKitCore.h>
#include <GUIKit/WindowManager.h>
#include <GUIKit/UnifiedFont.h>

#include "PageManager/PageManager.h"

namespace s3d::gui {
	void GUIKitCore::initialize() {
		m_pageManager = new PageManager();

		UnifiedFont::Initialize();

		WindowManager::Initialize();

		Scene::SetScaleMode(ScaleMode::ResizeFill);

		System::SetTerminationTriggers(UserAction::None);
	}

	void GUIKitCore::start() {
		if (m_pageManager->initialize()) {
			run();
		}
		else {
			Logger << U"Error(GUIKitCore): No pages are registered.";
		}

		delete m_pageManager;
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
