#include "PageManager.h"
#include <GUIKit/Page.h>
#include <GUIKit/WindowManager.h>
#include <GUIKit/GUIKitInterface.h>

namespace s3d::gui {
	PageManager::PageManager() {
		// Set scissor rect
		RasterizerState rasterizer = RasterizerState::Default2D;
		rasterizer.scissorEnable = true;
		Graphics2D::Internal::SetRasterizerState(rasterizer);
		m_windowScissorRect = Rect(0, 0, Window::ClientWidth(), Window::ClientHeight());
	}

	bool PageManager::initialize() {
		if (m_pages) {
			m_forwardPage = m_pages[0];
			return true;
		}

		return false;
	}

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

		if (GUIKit::IsTerminationPrevented()) {
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
	}

	void PageManager::updateLayers() {
		if (WindowManager::DidResized()) {
			// Update layer
			m_drawingPage->m_view.updateLayer(m_windowScissorRect);
			m_drawingPage->m_view.updateLayerInvert(m_windowScissorRect);

			// Update isolated components
			/*for (auto& component : m_isolatedComponents) {
				component->updateLayer(m_windowScissorRect);
			}*/

			// Call window resized event
			m_drawingPage->onWindowResized();
		}
		else {
			m_drawingPage->m_view.updateLayerIfNeeded(m_windowScissorRect);

			// Update isolated components
			/*for (auto& component : m_isolatedComponents) {
				component->updateLayerIfNeeded(m_windowScissorRect);
			}*/
		}
	}

	void PageManager::appendPage(const std::shared_ptr<Page>& page) {
		m_pages.push_back(page);
	}

	Page& PageManager::getPage(const String& identifier) const noexcept {
		const auto ptr = getPagePtr(identifier);
		auto& p = *ptr;
		return p;
	}

	std::shared_ptr<Page> PageManager::getPagePtr(const String& identifier) const {
		for (const auto& page : m_pages) {
			if (page->identifier() == identifier) {
				return page;
			}
		}

		Logger << U"Error(GUIKitCore): A page identified as {} does not exist."_fmt(identifier);

		assert(false);

		return nullptr;
	}

	void PageManager::switchPage(const String& identifier) {
		if (const auto& page = getPagePtr(identifier); m_pageTransition == PageTransition::Stable && page) {
			m_forwardPage = page;
			m_backwardPage = m_drawingPage;
			m_pageTransition = PageTransition::StartChanging;
		}
		else {
			Logger << U"Error(GUIKitCore): Switched current ui to the ui identified as {}, but the ui does not exist."_fmt(identifier);
		}
	}
}
