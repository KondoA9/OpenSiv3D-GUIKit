#include <GUIKit/GUIKitCore.h>
#include <GUIKit/Page.h>

#include <thread>

using namespace s3d::gui;

void GUIKit::switchPage(const String& identifier) {
	if (const auto& page = getPagePtr<Page>(identifier); !m_pageChanging && page) {
		m_forwardPage = page;
		m_backwardPage = m_drawingPage;
		m_preparePageChanging = true;
	}
	else {
		Logger << U"Error(GUIKitCore): Switched current ui to the ui identified as {}, but the ui does not exist."_fmt(identifier);
	}
}

void GUIKit::setColorMode(ColorMode mode) {
	m_animateColor = true;
	ColorTheme::SetColorMode(mode);
}

void GUIKit::toggleColorMode() {
	setColorMode(ColorTheme::CurrentColorMode() == ColorMode::Light ? ColorMode::Dark : ColorMode::Light);
}

bool GUIKit::animateColor() {
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

void GUIKit::insertToMainThread(const std::function<void()>& func) {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_eventsRequestedToRunInMainThread.push_back(func);
}

void GUIKit::insertAsyncProcess(const std::function<void()>& asyncFunc, const std::function<void()>& mainThreadFunc) {
	std::thread thread([this, asyncFunc, mainThreadFunc]() {
		asyncFunc();
		if (mainThreadFunc) {
			insertToMainThread(mainThreadFunc);
		}
		});
	thread.detach();
}

size_t GUIKit::setTimeout(const std::function<void()>& func, double ms, bool threading) {
	m_timeouts.push_back(Timeout(func, ms, threading));
	return m_timeouts[m_timeouts.size() - 1].id();
}

bool GUIKit::stopTimeout(size_t id) {
	for (auto& timeout : m_timeouts) {
		if (timeout.id() == id) {
			return timeout.stop();
		}
	}
	return false;
}

bool GUIKit::restartTimeout(size_t id) {
	for (auto& timeout : m_timeouts) {
		if (timeout.id() == id) {
			return timeout.restart();
		}
	}
	return false;
}

bool GUIKit::isTimeoutAlive(size_t id) {
	for (auto& timeout : m_timeouts) {
		if (timeout.id() == id) {
			return timeout.isAlive();
		}
	}
	return false;
}