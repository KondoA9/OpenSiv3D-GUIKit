#include <GUIKit/GUIKitCore.h>
#include <GUIKit/Page.h>
#include "PageManager/PageManager.h"

#include <thread>

namespace s3d::gui {
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

	void GUIKitCore::insertAsyncProcess(const std::function<void()>& func, const std::function<void()>& completion) {
		std::thread thread([this, func, completion]() {
			func();
			if (completion) {
				insertProcessToMainThread(completion);
			}
			});
		thread.detach();
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

	void GUIKitCore::appendIsolatedComponent(const std::shared_ptr<UIComponent>& component) {
		m_pageManager->appendIsolatedComponent(component);
	}
}
