#include <GUIKit/GUIKitCore.hpp>
#include <GUIKit/Page.hpp>
#include <GUIKit/WindowManager.hpp>
#include <GUIKit/UnifiedFont.hpp>
#include <GUIKit/GUIFactory.hpp>

#include "PageManager/PageManager.hpp"
#include "ParallelTaskManager/ParallelTaskManager.hpp"

#include <thread>

namespace s3d::gui {
	GUIKitCore::GUIKitCore() {
		m_pageManager = new PageManager();
		m_parallelTaskManager = new ParallelTaskManager();

		WindowManager::Initialize();

		UnifiedFont::Initialize();

		Scene::SetResizeMode(ResizeMode::Actual);

		System::SetTerminationTriggers(UserAction::NoAction);

		GUIKitCore::AddLicense();
	}

	GUIKitCore::~GUIKitCore() {
		delete m_pageManager;
		delete m_parallelTaskManager;
	}

	bool GUIKitCore::isParalellTaskAlive() const {
		return m_parallelTaskManager->isAlive();
	}

	void GUIKitCore::switchPage(const String& identifier) {
		m_pageManager->switchPage(identifier);
	}

	void GUIKitCore::setColorMode(ColorMode mode) {
		m_animateColor = true;
		ColorTheme::SetColorMode(mode);
	}

	void GUIKitCore::toggleColorMode() {
		setColorMode(ColorTheme::CurrentColorMode() == ColorMode::Light ? ColorMode::Dark : ColorMode::Light);
	}

	bool GUIKitCore::animateColor() {
		static double t = 0.0;
		t += 5.0 * Scene::DeltaTime();

		if (t > 1.0) {
			ColorTheme::Animate(ColorTheme::CurrentColorMode() == ColorMode::Light ? 0.0 : 1.0);
			t = 0.0;
			return false;
		}

		ColorTheme::Animate(ColorTheme::CurrentColorMode() == ColorMode::Light ? 1 - t : t);
		return true;
	}

	void GUIKitCore::insertProcessToMainThread(const std::function<void()>& func) {
		std::lock_guard<std::mutex> lock(m_mainThreadInserterMutex);
		m_eventsRequestedToRunInMainThread.push_back(func);
	}

	void GUIKitCore::createParallelTask(const std::function<void()>& func, const std::function<void()>& completion) {
		if (completion) {
			m_parallelTaskManager->createTask(func, [this, completion] {
				insertProcessToMainThread(completion);
				});
		}
		else {
			m_parallelTaskManager->createTask(func);
		}
	}

	size_t GUIKitCore::setTimeout(const std::function<void()>& func, double ms, bool threading) {
		m_timeouts.push_back(Timeout(func, ms, threading));
		return m_timeouts[m_timeouts.size() - 1].id();
	}

	bool GUIKitCore::stopTimeout(size_t id) {
		for (auto& timeout : m_timeouts) {
			if (timeout.id() == id) {
				return timeout.stop();
			}
		}
		return false;
	}

	bool GUIKitCore::restartTimeout(size_t id) {
		for (auto& timeout : m_timeouts) {
			if (timeout.id() == id) {
				return timeout.restart();
			}
		}
		return false;
	}

	bool GUIKitCore::isTimeoutAlive(size_t id) {
		for (auto& timeout : m_timeouts) {
			if (timeout.id() == id) {
				return timeout.isAlive();
			}
		}
		return false;
	}

	Page& GUIKitCore::getPage(const String& identifier) const noexcept {
		return m_pageManager->getPage(identifier);
	}

	void GUIKitCore::appendPage(const std::shared_ptr<Page>& page) {
		m_pageManager->appendPage(page);
	}

	void GUIKitCore::appendIsolatedComponent(const UIComponent& component) {
		appendIsolatedComponent(GUIFactory::GetComponent(component.id()));
	}

	void GUIKitCore::appendIsolatedComponent(const std::shared_ptr<UIComponent>& component) {
		m_pageManager->appendIsolatedComponent(component);
	}

	void GUIKitCore::AddLicense() {
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
