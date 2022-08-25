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
            m_nextPage      = m_pages[0].get();
            return true;
        }

        return false;
    }

    Page& PageManager::getPage(StringView identifier) const {
        return *m_pages[getPageIndex(identifier)];
    }

#pragma warning(disable : 4715)
    size_t PageManager::getPageIndex(StringView identifier) const {
        for (size_t i = 0; i < m_pages.size(); i++) {
            if (m_pages[i]->identifier() == identifier) {
                return i;
            }
        }

        AobaLog::Log(AobaLog::Type::Error, U"PageManager", U"A page identified as {} does not exist."_fmt(identifier));

        assert(false);
    }
#pragma warning(default : 4715)

    void PageManager::switchPage(StringView identifier) {
        if (const auto index = getPageIndex(identifier); m_pageTransition == PageTransition::Stable && m_pages[index]) {
            m_nextPage          = m_pages[index].get();
            m_previousPage      = m_currentPage;
            m_currentPage       = nullptr;
            m_pageTransition = PageTransition::StartChanging;
        } else {
            AobaLog::Log(AobaLog::Type::Error,
                         U"PageManager",
                         U"Switched current ui to the ui identified as {}, but the ui does not exist."_fmt(identifier));
        }
    }

    void PageManager::appendPage(std::unique_ptr<Page>&& page) {
        m_pages.emplace_back(std::move(page));
    }

    void PageManager::terminate() {
        m_pageTransition = PageTransition::Termination;
    }
}
