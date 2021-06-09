#include "GUIKit.h"

class StartPage : public gui::Page {
    using Page::Page;

    gui::UIText title;
    gui::UIButton nextButton;
    gui::UIButton switchThemeButton;
    gui::UIRect cover;
    gui::UISlider slider;
    gui::UIInputField inputField;

    void onLoaded() override {
        title = gui::UIText(U"GUIKit", gui::UnifiedFontStyle::Medium, gui::TextDirection::Center);
        title.setConstraint(gui::LayerDirection::Top);
        title.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom, 0.0, 0.5);
        title.setConstraint(gui::LayerDirection::Left);
        title.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);
        title.addEventListener(gui::MouseEventType::LeftDown, [](gui::MouseEvent e) {
            static bool flag = true;
            e.component->backgroundColor.setColor(flag ? gui::DynamicColor::BackgroundSecondary : gui::DynamicColor::Background, 2.0);
            flag = !flag;
            });

        nextButton = gui::UIButton(U"Next");
        nextButton.drawFrame = true;
        nextButton.setConstraint(gui::LayerDirection::Top, title, gui::LayerDirection::Bottom);
        nextButton.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom);
        nextButton.setConstraint(gui::LayerDirection::Left);
        nextButton.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right, 0.0, 0.5);
        nextButton.addEventListener(gui::MouseEventType::LeftDown, [this]() {
            m_guikit->switchPage(U"NextPage");
            });

        switchThemeButton = gui::UIButton(U"Switch theme");
        switchThemeButton.setConstraint(gui::LayerDirection::Top, nextButton, gui::LayerDirection::Top);
        switchThemeButton.setConstraint(gui::LayerDirection::Bottom, nextButton, gui::LayerDirection::Bottom);
        switchThemeButton.setConstraint(gui::LayerDirection::Left, nextButton, gui::LayerDirection::Right);
        switchThemeButton.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);
        switchThemeButton.addEventListener(gui::MouseEventType::LeftDown, [this]() {
            m_guikit->toggleColorMode();
            });

        cover.setConstraint(gui::LayerDirection::Height, switchThemeButton, gui::LayerDirection::Height, 0.0, 0.5);
        cover.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom);
        cover.setConstraint(gui::LayerDirection::Left);
        cover.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);

        slider.setLabel(U"Slider");
        slider.setConstraint(gui::LayerDirection::Height, switchThemeButton, gui::LayerDirection::Height, 0.0, 0.5);
        slider.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom);
        slider.setConstraint(gui::LayerDirection::Left);
        slider.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right, 0.0 , 0.5);

        inputField.setConstraint(gui::LayerDirection::Height, switchThemeButton, gui::LayerDirection::Height, 0.0, 0.5);
        inputField.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom);
        inputField.setConstraint(gui::LayerDirection::Left, slider, gui::LayerDirection::Right);
        inputField.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);

        m_view.appendComponent(title);
        m_view.appendComponent(nextButton);
        m_view.appendComponent(switchThemeButton);
        m_view.appendComponent(cover);
        m_view.appendComponent(slider);
        m_view.appendComponent(inputField);
    }
};

class NextPage : public gui::Page {
    using Page::Page;

    gui::UIButton backButton = gui::UIButton(U"Back");

    void onLoaded() override {
        backButton.setConstraint(gui::LayerDirection::Top);
        backButton.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom);
        backButton.setConstraint(gui::LayerDirection::Left);
        backButton.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);
        backButton.addEventListener(gui::MouseEventType::LeftDown, [this]() {
            m_guikit->switchPage(U"StartPage");
            });

        m_view.appendComponent(backButton);
    }
};

void Main() {
    gui::GUIKit guikit;
    guikit.appendPage(StartPage(U"StartPage"));
    guikit.appendPage(NextPage(U"NextPage"));
    guikit.start();
}