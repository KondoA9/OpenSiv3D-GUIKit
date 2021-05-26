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
}

void GUIKit::update() {
	WindowManager::Update();

	// Load start page
	static bool startup = true;
	if (startup && m_forwardPage) {
		static bool appeared = false;
		if (!appeared) {
			m_forwardPage->onLoaded();
			m_forwardPage->loaded = true;
			m_forwardPage->onBeforeAppeared();
			m_drawingPage = m_forwardPage;
			appeared = true;
		}
		else {
			m_forwardPage->onAfrerAppeared();
			m_forwardPage = nullptr;
			startup = false;
		}
	}

	// Page changing
	static bool forwardUILoaded = false;
	static double pageColorMultiplier = 1.0;
	if (m_forwardPage && m_backwardPage) {
		if (!forwardUILoaded) {
			if (!m_forwardPage->loaded) {
				m_forwardPage->onLoaded();
				m_forwardPage->loaded = true;
			}
			m_forwardPage->view.updateLayer();
			m_forwardPage->onBeforeAppeared();
			m_backwardPage->onBeforeDisappeared();
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
		m_forwardPage->onAfrerAppeared();
		m_backwardPage->onAfrerDisappeared();
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
			m_forwardPage->view.draw();
			uiChanged = true;
			m_uiChanging = false;
			pageColorMultiplier = 1.0;
		}
		else {
			Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0 - pageColorMultiplier));
			m_forwardPage->view.draw();
			Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, pageColorMultiplier));
			m_backwardPage->view.draw();
		}
	}
	// Draw the page
	else {
		m_drawingPage->view.updateMouseEvent();

		if (WindowManager::DidResized()) {
			m_drawingPage->onWindowResized();
			m_drawingPage->view.updateLayer();
		}
		else {
			m_drawingPage->view.updateLayerIfNeeded();
		}

		m_drawingPage->view.draw();
	}

	Graphics::SkipClearScreen();
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