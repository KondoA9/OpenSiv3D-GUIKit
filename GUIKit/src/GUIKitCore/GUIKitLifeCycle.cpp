#include <GUIKit/GUIKitCore.hpp>
#include <GUIKit/WindowManager.hpp>
#include <GUIKit/UnifiedFont.hpp>
#include <GUIKit/ColorTheme.hpp>

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

		GUIKitCore::AddLicence();
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

	void GUIKitCore::AddLicence() {
		LicenseInfo licence;
		licence.title = U"OpenSiv3D-GUIKit";
		licence.copyright = U"Copyright (c) 2021 Ekyu Kondo";
		licence.text =
UR"(Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)";

		LicenseManager::AddLicense(licence);
	}
}
