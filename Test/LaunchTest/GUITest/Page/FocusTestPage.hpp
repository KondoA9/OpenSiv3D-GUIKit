#pragma once

#include <Aoba/Aoba.hpp>

#include "../../Test.hpp"

class FocusTestPage final : public aoba::Page {
    using Page::Page;

    void onLoaded() override;
};

void FocusTestPage::onLoaded() {
    auto& rect = aoba::Factory::Create<aoba::UIRect>(view);

    const size_t focusTimeout = aoba::Core::SetTimeout(
        [] { Test::Assert(false, "Focused the component, but event was not triggered"); }, 3000, false);

    aoba::Core::SetTimeout(
        [&rect] {
            rect.focus();
            Test::Assert(rect.isFocused(), "Focused a component, but it is not focused");
        },
        1500,
        false);

    rect.addEventListener<aoba::Event::Component::Focused>([&rect, focusTimeout] {
        aoba::Core::StopTimeout(focusTimeout);

        const size_t unfocusTimeout = aoba::Core::SetTimeout(
            [] { Test::Assert(false, "UnFocused the component, but event was not triggered"); }, 3000, false);

        aoba::Core::SetTimeout(
            [&rect] {
                rect.unFocus();
                Test::Assert(!rect.isFocused(), "UnFocused a component, but it is focused");
            },
            1500,
            false);

        rect.addEventListener<aoba::Event::Component::UnFocused>(
            [unfocusTimeout] { aoba::Core::StopTimeout(unfocusTimeout); });
    });

    // 5sec later, terminate app
    aoba::Core::SetTimeout([] { aoba::Core::Terminate(); }, 5000, true);
}
