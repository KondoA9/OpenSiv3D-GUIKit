﻿#pragma once

#include <Siv3D.hpp>

#include "Aoba/UIComponent.hpp"

namespace s3d::aoba {
    class Page;

    class PageManager final {
        enum class PageTransition { StartUp, Stable, StartChanging, Changing, JustChanged, Termination };

    private:
        PageTransition m_pageTransition = PageTransition::StartUp;

        Array<std::unique_ptr<Page>> m_pages;
        Page *m_nextPage = nullptr, *m_currentPage = nullptr, *m_previousPage = nullptr;

        double m_pageTransitionRate = 1.0;

        Rect m_windowScissorRect;

    public:
        PageManager();

        PageManager(const PageManager&) = delete;

        PageManager(PageManager&&) = delete;

        PageManager& operator=(const PageManager&) = delete;

        PageManager& operator=(PageManager&&) = delete;

        Page& getPage(StringView identifier) const;

        bool initialize() noexcept;

        void update();

        void draw();

        void switchPage(StringView identifier);

        void appendPage(std::unique_ptr<Page>&& page);

        void terminate();

    private:
        size_t getPageIndex(StringView identifier) const;

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
