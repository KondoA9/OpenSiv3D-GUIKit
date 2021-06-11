#include "GUIKitCore.h"
#include "../Page/Page.h"
#include "../Window/WindowManager.h"
#include "../UserInterface/Font/UnifiedFont.h"

using namespace s3d::gui;

void GUIKit::initialize() {
	WindowManager::Initialize();
	UnifiedFont::Initialize();

	Scene::SetScaleMode(ScaleMode::ResizeFill);
}

void GUIKit::start() {
	if (m_pages.size() > 0) {
		m_forwardPage = m_pages[0];
		run();
	}
	else {
		Logger << U"Error(GUIKitCore): No pages are registered.";
	}
}

void GUIKit::run() {
	while (System::Update()) {
		update();
	}

	termination();
}

void GUIKit::update() {
	WindowManager::Update();

	// Load start page
	static bool startup = true;
	if (startup && m_forwardPage) {
		static bool appeared = false;
		if (!appeared) {
			m_forwardPage->onLoaded();
			m_forwardPage->m_loaded = true;
			m_forwardPage->onBeforeAppeared();
			m_drawingPage = m_forwardPage;
			appeared = true;
		}
		else {
			m_forwardPage->onAfterAppeared();
			m_forwardPage = nullptr;
			startup = false;
		}
	}

	// Page changing
	static bool forwardUILoaded = false;
	static double pageColorMultiplier = 1.0;
	if (m_forwardPage && m_backwardPage) {
		if (!forwardUILoaded) {
			if (!m_forwardPage->m_loaded) {
				m_forwardPage->onLoaded();
				m_forwardPage->m_loaded = true;
			}
			m_forwardPage->onBeforeAppeared();
			m_backwardPage->onBeforeDisappeared();
			m_forwardPage->m_view.updateLayer();
			m_forwardPage->m_view.updateLayerInvert();
			m_forwardPage->onLayoutCompleted();
			m_uiChanging = true;
			forwardUILoaded = true;
		}
	}

	// Color theme
	if (m_animateColor) {
		animateColor();
	}

	// Page changed
	static bool uiChanged = false;
	if (uiChanged) {
		m_forwardPage->onAfterAppeared();
		m_backwardPage->onAfterDisappeared();
		m_drawingPage = m_forwardPage;
		m_forwardPage.reset();
		m_backwardPage.reset();
		forwardUILoaded = false;
		uiChanged = false;
	}

	// Drawing pages while page changing
	if (m_uiChanging) {
		pageColorMultiplier -= 5.0 * Scene::DeltaTime();
		if (pageColorMultiplier < 0.0) {
			Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0));
			m_forwardPage->m_view.draw();
			uiChanged = true;
			m_uiChanging = false;
			pageColorMultiplier = 1.0;
		}
		else {
			Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0 - pageColorMultiplier));
			m_forwardPage->m_view.draw();
			Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, pageColorMultiplier));
			m_backwardPage->m_view.draw();
		}
	}
	// Draw the page
	else {
		UIComponent::ResetMouseEvents();
		m_drawingPage->m_view.update();
		m_drawingPage->m_view.updateMouseEvent();
		UIComponent::CallMouseEvents();

		if (WindowManager::DidResized()) {
			m_drawingPage->m_view.updateLayer();
			m_drawingPage->m_view.updateLayerInvert();
			m_drawingPage->onWindowResized();
		}
		else {
			m_drawingPage->m_view.updateLayerIfNeeded();
		}

		// Run inserted events
		for (auto& f : m_eventsRequestedToRunInMainThread) {
			f();
		}
		m_eventsRequestedToRunInMainThread.release();

		// Run timeouts
		for (auto& timeout : m_timeouts) {
			timeout.update();
		}

		// Draw
		m_drawingPage->m_view.draw();

		// Additional drawing events
		for (auto& f : m_drawingEvents) {
			f();
		}
		m_drawingEvents.release();
	}

	Graphics::SkipClearScreen();
}

void GUIKit::termination() {
	for (auto& page : m_pages) {
		page->onAppTerminated();
	}
}

void GUIKit::setTitle(const String& title) {
	m_title = title;
	Window::SetTitle(title);
}

void GUIKit::switchPage(const String& identifier) {
	if (const auto& ui = getUserInterface<Page>(identifier); !m_uiChanging && ui) {
		m_forwardPage = ui;
		m_backwardPage = m_drawingPage;
	}
	else {
		Logger << U"Error(GUIKitCore): Switched current ui to the ui identified as {}, but the ui does not exist."_fmt(identifier);
	}
}

void GUIKit::setColorMode(ColorMode mode) {
	m_animateColor = true;
	ColorTheme::setColorMode(mode);
}

void GUIKit::toggleColorMode() {
	setColorMode(ColorTheme::colorMode() == ColorMode::Light ? ColorMode::Dark : ColorMode::Light);
}

void GUIKit::animateColor() {
	static double t = 0.0;
	if (ColorTheme::colorMode() == ColorMode::Dark) {
		t += 5.0 * Scene::DeltaTime();
		if (t > 1.0) {
			t = 1.0;
			m_animateColor = false;
		}
	}
	else {
		t -= 5.0 * Scene::DeltaTime();
		if (t < 0.0) {
			t = 0.0;
			m_animateColor = false;
		}
	}
	ColorTheme::animate(t);
}

void GUIKit::insertToMainThread(const std::function<void()>& func) {
	std::lock_guard<std::mutex> lock(m_mtx);
	m_eventsRequestedToRunInMainThread.push_back(func);
}

void GUIKit::insertAsyncProcess(const std::function<void()>& heavyFunc, const std::function<void()>& uiUpdatingFunc) {
	std::thread thread([this, heavyFunc, uiUpdatingFunc]() {
		heavyFunc();
		insertToMainThread(uiUpdatingFunc);
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