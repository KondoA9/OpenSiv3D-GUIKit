#pragma once

#include <Aoba/Aoba.hpp>

#include "../../Test.hpp"

namespace _internal {
    class PageEvent {
    private:
        const std::string m_name;

        bool m_value = false;

    public:
        PageEvent(std::string_view name) : m_name(name) {}

        void call() {
            m_value = true;
            Test::LogWithMsg(m_name);
        }

        void validate() const {
            Test::Assert(m_value, Test::LogMessageTemplate() + "Failed: " + m_name);
        }
    };

    class PagingTester {
    private:
        Array<PageEvent> m_events = {PageEvent("onLoaded"),
                                     PageEvent("onBeforeAppeared"),
                                     PageEvent("onAfterAppeared"),
                                     PageEvent("onBeforeDisappeared"),
                                     PageEvent("onAfterDisappeared"),
                                     PageEvent("onBeforeAppTerminated"),
                                     PageEvent("onAppTerminated")};

    public:
        void loaded() {
            m_events[0].call();
        }

        void beforeAppeared() {
            m_events[1].call();
        }

        void afterAppeared() {
            m_events[2].call();
        }

        void beforeDisappeared() {
            m_events[3].call();
        }

        void afterDisappeared() {
            m_events[4].call();
        }

        void beforeAppTerminated() {
            m_events[5].call();
        }

        void appTerminated() {
            m_events[6].call();
        }

        void validate() {
            for (const auto& e : m_events) {
                e.validate();
            }
        }
    };
}

class LifeCycleTestPage final : public aoba::Page {
private:
    _internal::PagingTester m_pagingTester;

public:
    void validate() {
        m_pagingTester.validate();
    }

protected:
    using Page::Page;

    void onLoaded() override {
        // Toggle color mode test
        aoba::Core::SetTimeout(
            [] {
                aoba::ColorTheme::SetMode(aoba::ColorMode::Light);
                aoba::ColorTheme::ToggleMode();
            },
            2000,
            false);

        aoba::Core::SetTimeout(
            [this] {
                Test::Assert(aoba::ColorTheme::CurrentMode() == aoba::ColorMode::Dark, "Color mode must be dark");
                Test::Assert(view.backgroundColor == aoba::DynamicColor::Background.dark,
                             "Color mode is dark but color of the view is not dark color");
            },
            4000,
            false);

        // 5sec later, switch page
        aoba::Core::SetTimeout([] { aoba::Core::SwitchPage(U"ComponentsTest"); }, 5000, true);

        m_pagingTester.loaded();
    }

    void onBeforeAppeared() override {
        m_pagingTester.beforeAppeared();
    }

    void onAfterAppeared() override {
        m_pagingTester.afterAppeared();
    }

    void onBeforeDisappeared() override {
        m_pagingTester.beforeDisappeared();
    }

    void onAfterDisappeared() override {
        m_pagingTester.afterDisappeared();
    }

    void onBeforeAppTerminated() override {
        m_pagingTester.beforeAppTerminated();
    }

    void onAppTerminated() override {
        m_pagingTester.appTerminated();
    }
};
