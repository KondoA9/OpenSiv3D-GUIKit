#include <Aoba/AobaCore.hpp>
#include <Aoba/Page.hpp>
#include <Aoba/WindowManager.hpp>
#include <Aoba/UnifiedFont.hpp>
#include <Aoba/AobaFactory.hpp>

#include "PageManager/PageManager.hpp"
#include "ParallelTaskManager/ParallelTaskManager.hpp"

#include <thread>

namespace s3d::aoba {
	AobaCore::AobaCore() {
		m_pageManager = new PageManager();
		m_parallelTaskManager = new ParallelTaskManager();

		WindowManager::Initialize();

		UnifiedFont::Initialize();

		Scene::SetResizeMode(ResizeMode::Actual);

		System::SetTerminationTriggers(UserAction::NoAction);

		AobaCore::AddLicense();
	}

	AobaCore::~AobaCore() {
		delete m_pageManager;
		delete m_parallelTaskManager;
	}

	bool AobaCore::isParalellTaskAlive() const {
		return m_parallelTaskManager->isAlive();
	}

	void AobaCore::switchPage(const String& identifier) {
		m_pageManager->switchPage(identifier);
	}

	void AobaCore::setColorMode(ColorMode mode) {
		m_animateColor = true;
		ColorTheme::SetColorMode(mode);
	}

	void AobaCore::toggleColorMode() {
		setColorMode(ColorTheme::CurrentColorMode() == ColorMode::Light ? ColorMode::Dark : ColorMode::Light);
	}

	bool AobaCore::animateColor() {
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

	void AobaCore::insertProcessToMainThread(const std::function<void()>& func) {
		std::lock_guard<std::mutex> lock(m_mainThreadInserterMutex);
		m_eventsRequestedToRunInMainThread.push_back(func);
	}

	void AobaCore::createParallelTask(const std::function<void()>& func, const std::function<void()>& completion) {
		if (completion) {
			m_parallelTaskManager->createTask(func, [this, completion] {
				insertProcessToMainThread(completion);
				});
		}
		else {
			m_parallelTaskManager->createTask(func);
		}
	}

	size_t AobaCore::setTimeout(const std::function<void()>& func, double ms, bool threading) {
		m_timeouts.push_back(Timeout(func, ms, threading));
		return m_timeouts[m_timeouts.size() - 1].id();
	}

	bool AobaCore::stopTimeout(size_t id) {
		for (auto& timeout : m_timeouts) {
			if (timeout.id() == id) {
				return timeout.stop();
			}
		}
		return false;
	}

	bool AobaCore::restartTimeout(size_t id) {
		for (auto& timeout : m_timeouts) {
			if (timeout.id() == id) {
				return timeout.restart();
			}
		}
		return false;
	}

	bool AobaCore::isTimeoutAlive(size_t id) {
		for (auto& timeout : m_timeouts) {
			if (timeout.id() == id) {
				return timeout.isAlive();
			}
		}
		return false;
	}

	Page& AobaCore::getPage(const String& identifier) const noexcept {
		return m_pageManager->getPage(identifier);
	}

	void AobaCore::appendPage(const std::shared_ptr<Page>& page) {
		m_pageManager->appendPage(page);
	}

	void AobaCore::appendIsolatedComponent(const UIComponent& component) {
		appendIsolatedComponent(AobaFactory::GetComponent(component.id()));
	}

	void AobaCore::appendIsolatedComponent(const std::shared_ptr<UIComponent>& component) {
		m_pageManager->appendIsolatedComponent(component);
	}

	void AobaCore::AddLicense() {
		LicenseInfo licence;
		licence.title = U"Aoba Framework";
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
