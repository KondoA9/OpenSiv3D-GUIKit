#include "PageManager.hpp"
#include <Aoba/Page.hpp>
#include <Aoba/WindowManager.hpp>
#include <Aoba/UIView.hpp>
#include <Aoba/Core.hpp>

namespace s3d::aoba {
	void PageManager::update() {
		// Update scissor rect if window is resized
		if (WindowManager::DidResized()) {
			const auto size = Scene::Size();
			m_windowScissorRect = Rect(0, 0, size.x, size.y);
		}

		// Update views
		if (m_forwardPage) {
			m_forwardPage->view.update();
		}

		if (m_drawingPage) {
			m_drawingPage->view.update();
		}

		if (m_backwardPage) {
			m_backwardPage->view.update();
		}

		// Page transition
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
	}

	void PageManager::draw() {
		switch (m_pageTransition)
		{
		case PageTransition::Changing:
			// Draw previous and next page
			Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, 1.0 - m_pageTransitionRate));
			m_forwardPage->view.draw();
			Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, m_pageTransitionRate));
			m_backwardPage->view.draw();
			break;

		case PageTransition::JustChanged:
			// Initialize ColorMultipiler
			Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, 1.0));
			m_forwardPage->view.draw();
			break;

		default:
			// Draw current page
			m_drawingPage->view.draw();
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
		// Run once when launching the app
		if (static bool called = false; !called) {
			m_forwardPage->onLoaded();
			m_forwardPage->onBeforeAppeared();
			m_drawingPage = m_forwardPage;
			called = true;
		}
		// Run at next frame
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

		if (aoba::Core::IsTerminationPrevented() || aoba::Core::IsParallelTaskAlive()) {
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
		// Load the next page if needed
		if (!m_forwardPage->didLoaded()) {
			m_forwardPage->onLoaded();
		}

		m_forwardPage->onBeforeAppeared();
		m_backwardPage->onBeforeDisappeared();

		// Update layer of the next page
		m_forwardPage->view.updateLayer(m_windowScissorRect);
		m_forwardPage->view.updateLayerInvert(m_windowScissorRect);
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
		if (m_drawingPage->view.eventUpdatable()) {
			m_drawingPage->view.updateMouseIntersection();
			m_drawingPage->view.updateInputEvents();
		}

		for (auto& component : m_isolatedComponents) {
			if (component->eventUpdatable()) {
				component->updateMouseIntersection();
				component->updateInputEvents();
			}
		}

		UIComponent::CallInputEvents();
	}

	void PageManager::updateLayers() {
		if (WindowManager::DidResized()) {
			// Update layer
			m_drawingPage->view.updateLayer(m_windowScissorRect);
			m_drawingPage->view.updateLayerInvert(m_windowScissorRect);

			// Update isolated components
			for (auto& component : m_isolatedComponents) {
				component->updateLayer(m_windowScissorRect);
			}

			// Call window resized event
			m_drawingPage->onWindowResized();
		}
		else {
			m_drawingPage->view.updateLayerIfNeeded(m_windowScissorRect);

			// Update isolated components
			for (auto& component : m_isolatedComponents) {
				component->updateLayerIfNeeded(m_windowScissorRect);
			}
		}
	}
}
