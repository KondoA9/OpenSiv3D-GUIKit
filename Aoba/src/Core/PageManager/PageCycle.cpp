#include "PageManager.hpp"

#include <Aoba/Core.hpp>
#include <Aoba/Page.hpp>
#include <Aoba/UIView.hpp>
#include <Aoba/WindowManager.hpp>

namespace s3d::aoba {
	void PageManager::update() {
		if (WindowManager::DidResized()) {
			// Update scissor rect
			const auto size = Scene::Size();
			m_windowScissorRect = Rect(0, 0, size.x, size.y);

			// Call window resize event
			if (m_nextPage) {
				m_nextPage->onWindowResized();
			}

			if (m_currentPage) {
				m_currentPage->onWindowResized();
			}

			if (m_previousPage) {
				m_previousPage->onWindowResized();
			}
		}

		updateViews();

		updateLayers();

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
		case PageTransition::StartChanging:
			m_previousPage->view.draw();
			break;

		case PageTransition::Changing:
			// Draw previous and next page
			Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, 1.0 - m_pageTransitionRate));
			m_nextPage->view.draw();
			Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, m_pageTransitionRate));
			m_previousPage->view.draw();
			break;

		case PageTransition::JustChanged:
			// Initialize ColorMultipiler
			Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, 1.0));
			m_nextPage->view.draw();
			break;

		default:
			// Draw current page
			m_currentPage->view.draw();
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
			m_currentPage->onLoaded();
			m_currentPage->onBeforeAppeared();
			called = true;
		}
		// Run at next frame
		else {
			m_currentPage->onAfterAppeared();
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

	void PageManager::updateOnStable() {
		assert(m_currentPage);

		updateInputEvents();
	}

	void PageManager::updateOnTermination() {
		static bool once = true;

		if (once) {
			for (auto& page : m_pages) {
				page->onBeforeAppTerminated();
			}

			once = false;
		}

		if (!(aoba::Core::IsTerminationPrevented() || aoba::Core::IsParallelTaskAlive())) {
			m_currentPage->onBeforeDisappeared();

			for (auto& page : m_pages) {
				page->onAppTerminated();
			}

			System::Exit();
		}
	}

	void PageManager::preparePageChanging() {
		// Load the next page if needed
		if (!m_nextPage->didLoaded()) {
			m_nextPage->onLoaded();
		}

		m_nextPage->onBeforeAppeared();
		m_previousPage->onBeforeDisappeared();

		// Request to update layer of the next page
		m_nextPage->view.requestToUpdateLayer();
	}

	void PageManager::finalizePageChanging() {
		m_nextPage->onAfterAppeared();
		m_previousPage->onAfterDisappeared();

		m_currentPage = m_nextPage;

		m_nextPage.reset();
		m_previousPage.reset();
	}

	void PageManager::updateViews() {
		if (m_nextPage) {
			m_nextPage->view.update();
		}

		if (m_currentPage) {
			m_currentPage->view.update();
		}

		if (m_previousPage) {
			m_previousPage->view.update();
		}
	}

	void PageManager::updateLayers() {
		if (WindowManager::DidResized()) {
			if (m_nextPage) {
				m_nextPage->view.updateLayer(m_windowScissorRect);
				m_nextPage->view.updateLayerInvert(m_windowScissorRect);
			}

			if (m_currentPage) {
				m_currentPage->view.updateLayer(m_windowScissorRect);
				m_currentPage->view.updateLayerInvert(m_windowScissorRect);
			}

			if (m_previousPage) {
				m_previousPage->view.updateLayer(m_windowScissorRect);
				m_previousPage->view.updateLayerInvert(m_windowScissorRect);
			}

			for (auto& component : m_isolatedComponents) {
				component->updateLayer(m_windowScissorRect);
			}
		}
		else {
			if (m_nextPage) {
				m_nextPage->view.updateLayerIfNeeded(m_windowScissorRect);
			}

			if (m_currentPage) {
				m_currentPage->view.updateLayerIfNeeded(m_windowScissorRect);
			}

			if (m_previousPage) {
				m_previousPage->view.updateLayerIfNeeded(m_windowScissorRect);
			}

			for (auto& component : m_isolatedComponents) {
				component->updateLayerIfNeeded(m_windowScissorRect);
			}
		}
	}

	void PageManager::updateInputEvents() {
		if (m_currentPage->view.eventUpdatable()) {
			m_currentPage->view.updateMouseIntersection();
			m_currentPage->view.updateInputEvents();
		}

		for (auto& component : m_isolatedComponents) {
			if (component->eventUpdatable()) {
				component->updateMouseIntersection();
				component->updateInputEvents();
			}
		}

		UIComponent::CallInputEvents();
	}
}
