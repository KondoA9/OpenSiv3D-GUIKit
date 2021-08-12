#include "PageManager.hpp"
#include <GUIKit/Page.hpp>
#include <GUIKit/WindowManager.hpp>
#include <GUIKit/GUIKitInterface.hpp>

namespace s3d::gui {
	void PageManager::update() {
		if (WindowManager::DidResized()) {
			// Update scissor rect
			m_windowScissorRect = Rect(0, 0, Window::ClientWidth(), Window::ClientHeight());
		}

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

		case PageTransition::Termination:
			updateOnTermination();
			break;

		default:
			break;
		}

		for (auto& component : m_isolatedComponents) {
			if (component->updatable()) {
				component->updateMouseIntersection();
				component->updateInputEvents();
			}
		}
	}

	void PageManager::draw() {
		switch (m_pageTransition)
		{
		case PageTransition::Changing:
			// Draw previous and next page
			Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0 - m_pageTransitionRate));
			m_forwardPage->m_view.draw();
			Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, m_pageTransitionRate));
			m_backwardPage->m_view.draw();
			break;

		case PageTransition::JustChanged:
			// Initialize ColorMultipiler
			Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0));
			m_forwardPage->m_view.draw();
			break;

		default:
			// Draw current page
			m_drawingPage->m_view.draw();
			break;
		}

		// Draw isolated components
		for (auto& component : m_isolatedComponents) {
			if (component->drawable()) {
				component->draw();
			}
		}
	}

	bool PageManager::updateOnStartUp() {
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

	bool PageManager::updateOnPageChanging() {
		m_pageTransitionRate -= 5.0 * Scene::DeltaTime();

		// The page changed
		if (m_pageTransitionRate < 0.0) {
			m_pageTransitionRate = 1.0;
			return false;
		}

		return true;
	}

	void PageManager::updateOnTermination() {
		static bool once = true;

		if (once) {
			for (auto& page : m_pages) {
				page->onBeforeAppTerminated();
			}

			once = false;
		}

		if (GUIKit::IsTerminationPrevented() || GUIKit::IsAsyncProcessAlive()) {
			updateLayers();
		}
		else {
			m_drawingPage->onBeforeDisappeared();
			for (auto& page : m_pages) {
				page->onAppTerminated();
			}

			System::Exit();
		}
	}

	void PageManager::preparePageChanging() {
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

	void PageManager::finalizePageChanging() {
		m_forwardPage->onAfterAppeared();
		m_backwardPage->onAfterDisappeared();

		m_drawingPage = m_forwardPage;

		m_forwardPage.reset();
		m_backwardPage.reset();
	}

	void PageManager::updateOnStable() {
		assert(m_drawingPage);

		updateInputEvents();

		updateLayers();
	}

	void PageManager::updateInputEvents() {
		if (m_drawingPage->m_view.updatable()) {
			m_drawingPage->m_view.updateMouseIntersection();
			m_drawingPage->m_view.updateInputEvents();
		}

		UIComponent::CallInputEvents();
	}

	void PageManager::updateLayers() {
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
}
