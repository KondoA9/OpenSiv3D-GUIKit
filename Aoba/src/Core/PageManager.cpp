#include "PageManager.hpp"

#include "Aoba/Page.hpp"
#include "src/AobaLog/AobaLog.hpp"

namespace s3d::aoba {
    PageManager::PageManager() {
        // Set scissor rect
        RasterizerState rasterizer = RasterizerState::Default2D;
        rasterizer.scissorEnable   = true;
        Graphics2D::Internal::SetRasterizerState(rasterizer);

        const auto size     = Window::GetState().frameBufferSize;
        m_windowScissorRect = Rect(0, 0, size.x, size.y);
    }

    bool PageManager::initialize() noexcept {
        if (m_pages) {
            // The following code does not throw an exception because the page is not empty.
            m_nextPage = m_pages[0];
            return true;
        }

        return false;
    }

    Page& PageManager::getPage(const String& identifier) const {
        const auto& ptr = getPagePtr(identifier);
        auto& p         = *ptr;
        return p;
    }

#pragma warning(disable : 4715)
    const std::shared_ptr<Page>& PageManager::getPagePtr(const String& identifier) const {
        for (const auto& page : m_pages) {
            if (page->identifier() == identifier) {
                return page;
            }
        }

        AobaLog::Log(AobaLog::Type::Error, U"PageManager", U"A page identified as {} does not exist."_fmt(identifier));

        assert(false);
    }
#pragma warning(default : 4715)

    void PageManager::switchPage(const String& identifier) {
        if (const auto& page = getPagePtr(identifier); m_pageTransition == PageTransition::Stable && page) {
            m_nextPage     = page;
            m_previousPage = m_currentPage;
            m_currentPage.reset();
            m_pageTransition = PageTransition::StartChanging;
        } else {
            AobaLog::Log(AobaLog::Type::Error,
                         U"PageManager",
                         U"Switched current ui to the ui identified as {}, but the ui does not exist."_fmt(identifier));
        }
    }

    void PageManager::appendPage(const std::shared_ptr<Page>& page) {
        m_pages.emplace_back(page);
    }

    void PageManager::terminate() {
        m_pageTransition = PageTransition::Termination;
    }
}
