#pragma once

#include <Aoba/Aoba.hpp>

#include "../../Test.hpp"

class ComponentsTestPage final : public aoba::Page {
    using Page::Page;

    void onLoaded() override;
};

void ComponentsTestPage::onLoaded() {
    auto& stackView = aoba::Factory::Create<aoba::UIVStackView>(view);

    auto& rect         = aoba::Factory::Create<aoba::UIRect>(stackView);
    auto& circle       = aoba::Factory::Create<aoba::UICircle>(stackView);
    auto& button       = aoba::Factory::Create<aoba::UIButton>(stackView);
    auto& toggleButton = aoba::Factory::Create<aoba::UIToggleButton>(stackView);
    auto& text         = aoba::Factory::Create<aoba::UIText>(stackView);
    auto& inputField   = aoba::Factory::Create<aoba::UIInputField>(stackView);
    auto& checkBox     = aoba::Factory::Create<aoba::UICheckBox>(stackView);
    auto& slider       = aoba::Factory::Create<aoba::UISlider>(stackView);
    auto& icon         = aoba::Factory::Create<aoba::UIIcon>(stackView);
    auto& imageView    = aoba::Factory::Create<aoba::UIImageView>(stackView);

    auto& tabView = aoba::Factory::Create<aoba::UITabView>(view);
    auto& tab1    = tabView.appendTab<aoba::UIView>(U"Tab1");
    auto& tab2    = tabView.appendTab<aoba::UIView>(U"Tab2");

    auto& verticalAlignView      = aoba::Factory::Create<aoba::UIView>(view);
    auto& verticalAlignViewRect1 = aoba::Factory::Create<aoba::UIRect>(verticalAlignView);
    auto& verticalAlignViewRect2 = aoba::Factory::Create<aoba::UIRect>(verticalAlignView);

    view.layout<aoba::AlignH>(
        {aoba::LORatio(stackView, 0.4),
         aoba::LORatio(tabView, 0.2),
         aoba::LOFill(verticalAlignView.layout<aoba::AlignV>(
             {aoba::LOConstant(verticalAlignViewRect1, 200_px), aoba::LOFill(verticalAlignViewRect2)}))});

    rect.backgroundColor         = aoba::MaterialColor::Amber5;
    circle.backgroundColor       = aoba::MaterialColor::Blue5;
    button.backgroundColor       = aoba::MaterialColor::BlueGrey5;
    toggleButton.backgroundColor = aoba::MaterialColor::Brown5;
    text.backgroundColor         = aoba::MaterialColor::Cyan5;
    inputField.backgroundColor   = aoba::MaterialColor::DeepOrange5;
    checkBox.backgroundColor     = aoba::MaterialColor::DeepPurple5;
    slider.backgroundColor       = aoba::MaterialColor::Grey5;
    icon.backgroundColor         = aoba::MaterialColor::Green5;
    imageView.backgroundColor    = aoba::MaterialColor::Indigo5;

    tab1.backgroundColor = aoba::MaterialColor::Blue5;
    tab2.backgroundColor = aoba::MaterialColor::Blue5;

    verticalAlignViewRect1.backgroundColor = aoba::MaterialColor::Green3;
    verticalAlignViewRect2.backgroundColor = aoba::MaterialColor::Green7;

    button.setText(U"UIButton");
    toggleButton.setText(U"UIToggleButton");
    text.setText(U"UIText");
    inputField.setText(U"UIInputField");
    checkBox.setChecked(true);
    icon.setIcon(Icon(0xf109), 40_px);
    imageView.appendImage(Image(U"asset/image1.png"));

    // 5sec later, switch page
    aoba::Core::SetTimeout([] { aoba::Core::SwitchPage(U"FocusTest"); }, 5000, true);
}
