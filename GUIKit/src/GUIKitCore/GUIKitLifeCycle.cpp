#include <GUIKit/GUIKitCore.h>
#include <GUIKit/WindowManager.h>
#include <GUIKit/UnifiedFont.h>

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

	// Start up
	if (static bool startup = true; startup) {
		startup = updateOnStartUp();
	}

	// Prepare for page changing
	if (m_preparePageChanging) {
		preparePageChanging();
		m_preparePageChanging = false;
		m_pageChanging = true;
	}

	// Color theme
	if (m_animateColor) {
		m_animateColor = animateColor();
	}

	// Page changed
	static bool pageJustChanged = false;
	if (pageJustChanged) {
		m_forwardPage->onAfterAppeared();
		m_backwardPage->onAfterDisappeared();
		m_drawingPage = m_forwardPage;
		m_forwardPage.reset();
		m_backwardPage.reset();
		pageJustChanged = false;
		m_pageChanging = false;
	}

	// Drawing pages while page changing
	if (m_pageChanging) {
		pageJustChanged = !updateOnPageChanging();
	}
	// Draw the page
	else {
		updateOnStable();
	}

	Graphics::SkipClearScreen();
}

bool GUIKit::updateOnStartUp() {
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
		m_forwardPage.reset();
		return false;
	}
	return true;
}

bool GUIKit::updateOnPageChanging() {
	static double pageColorMultiplier = 1.0;
	pageColorMultiplier -= 5.0 * Scene::DeltaTime();

	// The page changed
	if (pageColorMultiplier < 0.0) {
		Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0));
		m_forwardPage->m_view.draw();
		pageColorMultiplier = 1.0;
		return false;
	}
	// Pages are changing
	else {
		Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0 - pageColorMultiplier));
		m_forwardPage->m_view.draw();
		Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, pageColorMultiplier));
		m_backwardPage->m_view.draw();
	}

	return true;
}

void GUIKit::updateOnStable() {
	// Update mouse events
	UIComponent::ResetMouseEvents();
	m_drawingPage->m_view.updateMouseIntersection();
	m_drawingPage->m_view.updateMouseEvents();
	UIComponent::CallMouseEvents();

	// Window resized event
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
	{
		bool alive = false;
		for (auto& timeout : m_timeouts) {
			timeout.update();
			alive |= timeout.isAlive();
		}
		if (!alive) {
			m_timeouts.release();
		}
	}

	// Draw
	m_drawingPage->m_view.draw();

	// Additional drawing events
	for (auto& f : m_drawingEvents) {
		f();
	}
	m_drawingEvents.release();
}

void GUIKit::preparePageChanging() {
	// Load a page once
	if (!m_forwardPage->m_loaded) {
		m_forwardPage->onLoaded();
		m_forwardPage->m_loaded = true;
	}

	m_forwardPage->onBeforeAppeared();
	m_backwardPage->onBeforeDisappeared();
	m_forwardPage->m_view.updateLayer();
	m_forwardPage->m_view.updateLayerInvert();
	m_forwardPage->onLayoutCompleted();
}

void GUIKit::termination() {
	for (auto& page : m_pages) {
		page->onAppTerminated();
	}
}