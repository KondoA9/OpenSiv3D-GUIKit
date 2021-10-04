#include "PageManager.hpp"
#include <GUIKit/Page.hpp>

namespace s3d::gui {
	PageManager::PageManager() {
		// Set scissor rect
		RasterizerState rasterizer = RasterizerState::Default2D;
		rasterizer.scissorEnable = true;
		Graphics2D::Internal::SetRasterizerState(rasterizer);

		const auto size = Window::GetState().frameBufferSize;
		m_windowScissorRect = Rect(0, 0, size.x, size.y);
	}

	bool PageManager::initialize() {
		if (m_pages) {
			m_forwardPage = m_pages[0];
			return true;
		}

		return false;
	}

	Page& PageManager::getPage(const String& identifier) const noexcept {
		const auto& ptr = getPagePtr(identifier);
		auto& p = *ptr;
		return p;
	}

#pragma warning(disable: 4715)
	const std::shared_ptr<Page>& PageManager::getPagePtr(const String& identifier) const {
		for (const auto& page : m_pages) {
			if (page->identifier() == identifier) {
				return page;
			}
		}

		Logger << U"Error(PageManager): A page identified as {} does not exist."_fmt(identifier);

		assert(false);
	}
#pragma warning(default: 4715)

	void PageManager::switchPage(const String& identifier) {
		if (const auto& page = getPagePtr(identifier); m_pageTransition == PageTransition::Stable && page) {
			m_forwardPage = page;
			m_backwardPage = m_drawingPage;
			m_pageTransition = PageTransition::StartChanging;
		}
		else {
			Logger << U"Error(PageManager): Switched current ui to the ui identified as {}, but the ui does not exist."_fmt(identifier);
		}
	}
}
