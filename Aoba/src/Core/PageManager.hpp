#pragma once

#include <Siv3D.hpp>

#include "Aoba/UIComponent.hpp"

namespace s3d::aoba {
    class Page;

    class PageManager final {
        enum class PageTransition { StartUp, Stable, StartChanging, Changing, JustChanged, Termination };

    private:
        PageTransition m_pageTransition = PageTransition::StartUp;

        Array<std::shared_ptr<Page>> m_pages;
        std::shared_ptr<Page> m_nextPage, m_currentPage, m_previousPage;

        double m_pageTransitionRate = 1.0;

        Rect m_windowScissorRect;

    public:
        PageManager();

        PageManager(const PageManager&) = delete;

        PageManager(PageManager&&) = delete;

        PageManager& operator=(const PageManager&) = delete;

        PageManager& operator=(PageManager&&) = delete;

        Page& getPage(const String& identifier) const noexcept;

        bool initialize();

        void update();

        void draw();

        void switchPage(const String& identifier);

        void appendPage(const std::shared_ptr<Page>& page);

        void terminate();

    private:
        const std::shared_ptr<Page>& getPagePtr(const String& identifier) const;

        // Return true until the start up page appeared.
        bool updateOnStartUp();

        // Return true until the page changed.
        bool updateOnPageChanging();

        void updateOnStable();

        void preparePageChanging();

        void finalizePageChanging();

        void updateOnTermination();

        void updateComponents();

        void updateLayers();

        void updateInputEvents();
    };
}
