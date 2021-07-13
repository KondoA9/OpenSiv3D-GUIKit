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
	// Set scissor rect
	{
		RasterizerState rasterizer = RasterizerState::Default2D;
		rasterizer.scissorEnable = true;
		Graphics2D::Internal::SetRasterizerState(rasterizer);
		m_windowScissorRect = Rect(0, 0, Window::ClientWidth(), Window::ClientHeight());
	}

	while (System::Update()) {
		updateGUIKit();
	}

	termination();
}

void GUIKit::updateGUIKit() {
	// Update window state
	WindowManager::Update();
	if (WindowManager::DidResized()) {
		// Update scissor rect
		m_windowScissorRect = Rect(0, 0, Window::ClientWidth(), Window::ClientHeight());
	}

	// Update pages
	update();

	// Update color theme
	if (m_animateColor) {
		m_animateColor = animateColor();
	}

	// Draw pages, components and events
	draw();
}

void GUIKit::termination() {
	for (auto& page : m_pages) {
		page->onAppTerminated();
	}
}

void GUIKit::update() {
	switch (m_pageTransition)
	{
	case PageTransition::StartUp:
		if (!updateOnStartUp()) {
			m_pageTransition = PageTransition::Stable;
		}
		break;

	case PageTransition::Stable:
		updateOnStable();
		break;

	case PageTransition::StartChanging:
		preparePageChanging();
		m_pageTransition = PageTransition::Changing;
		break;

	case PageTransition::Changing:
		if (!updateOnPageChanging()) {
			m_pageTransition = PageTransition::JustChanged;
		}
		break;

	case PageTransition::JustChanged:
		finalizePageChanging();
		m_pageTransition = PageTransition::Stable;
		break;

	default:
		break;
	}
}

void GUIKit::draw() {
	switch (m_pageTransition)
	{
	case s3d::gui::GUIKit::PageTransition::Changing:
		// Draw previous and next page
		Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0 - m_pageTransitionRate));
		m_forwardPage->m_view.draw();
		Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, m_pageTransitionRate));
		m_backwardPage->m_view.draw();
		break;

	case s3d::gui::GUIKit::PageTransition::JustChanged:
		// Initialize ColorMultipiler
		Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0));
		m_forwardPage->m_view.draw();
		break;

	default:
		// Draw current page
		m_drawingPage->m_view.draw();

		// Draw isolated components
		for (auto& component : m_isolatedComponents) {
			if (component->drawable()) {
				component->draw();
			}
		}
		break;
	}

	// Additional drawing events
	for (auto& f : m_drawingEvents) {
		f();
	}
	m_drawingEvents.release();
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
	m_pageTransitionRate -= 5.0 * Scene::DeltaTime();

	// The page changed
	if (m_pageTransitionRate < 0.0) {
		m_pageTransitionRate = 1.0;
		return false;
	}

	return true;
}

void GUIKit::preparePageChanging() {
	// Load a page once
	if (!m_forwardPage->m_loaded) {
		m_forwardPage->onLoaded();
		m_forwardPage->m_loaded = true;
	}

	m_forwardPage->onBeforeAppeared();
	m_backwardPage->onBeforeDisappeared();

	m_forwardPage->m_view.updateLayer(m_windowScissorRect);
	m_forwardPage->m_view.updateLayerInvert(m_windowScissorRect);

	m_forwardPage->onLayoutCompleted();
}

void GUIKit::finalizePageChanging() {
	m_forwardPage->onAfterAppeared();
	m_backwardPage->onAfterDisappeared();

	m_drawingPage = m_forwardPage;

	m_forwardPage.reset();
	m_backwardPage.reset();
}

void GUIKit::updateOnStable() {
	assert(m_drawingPage);

	updateInputEventsStable();

	updateLayerStable();

	updateMainThreadEventsStable();

	updateTimeoutsStable();
}

void GUIKit::updateInputEventsStable() {
	UIComponent::ResetInputEvents();
	if (m_drawingPage->m_view.updatable()) {
		m_drawingPage->m_view.updateMouseIntersection();
		m_drawingPage->m_view.updateInputEvents();
	}
	for (auto& component : m_isolatedComponents) {
		if (component->updatable()) {
			component->updateMouseIntersection();
			component->updateInputEvents();
		}
	}
	UIComponent::CallInputEvents();
}

void GUIKit::updateLayerStable() {
	if (WindowManager::DidResized()) {
		// Update layer
		m_drawingPage->m_view.updateLayer(m_windowScissorRect);
		m_drawingPage->m_view.updateLayerInvert(m_windowScissorRect);

		// Update isolated components
		for (auto& component : m_isolatedComponents) {
			component->updateLayer(m_windowScissorRect);
		}

		// Call window resized event
		m_drawingPage->onWindowResized();
	}
	else {
		m_drawingPage->m_view.updateLayerIfNeeded(m_windowScissorRect);

		// Update isolated components
		for (auto& component : m_isolatedComponents) {
			component->updateLayerIfNeeded(m_windowScissorRect);
		}
	}
}

void GUIKit::updateMainThreadEventsStable() {
	std::lock_guard<std::mutex> lock(m_mainThreadInserterMutex);

	for (const auto& f : m_eventsRequestedToRunInMainThread) {
		f();
	}

	m_eventsRequestedToRunInMainThread.release();
}

void GUIKit::updateTimeoutsStable() {
	bool alive = false;

	for (auto& timeout : m_timeouts) {
		timeout.update();
		alive |= timeout.isAlive();
	}

	if (!alive) {
		m_timeouts.release();
	}
}
