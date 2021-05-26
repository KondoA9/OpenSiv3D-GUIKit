#include "GUIKit.h"

class StartPage : public gui::Page {
    using Page::Page;

    gui::UIText title;
    gui::UIButton nextButton;
    gui::UIButton switchThemeButton;

    void onLoaded() override {
        title = gui::UIText(U"GUIKit", gui::UnifiedFontStyle::Medium, gui::TextDirection::Center);
        title.setConstraint(gui::LayerDirection::Top);
        title.setConstraint(gui::LayerDirection::Bottom, view, gui::LayerDirection::Bottom, 0.0, 0.5);
        title.setConstraint(gui::LayerDirection::Left);
        title.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);
        title.addEventListener(gui::MouseEventType::Down, [](gui::MouseEvent e) {
            static bool flag = true;
            e.component->backgroundColor.setColor(flag ? gui::DynamicColor::BackgroundSecondary : gui::DynamicColor::Background, 2.0);
            flag = !flag;
            });

        nextButton = gui::UIButton(U"Next");
        nextButton.setConstraint(gui::LayerDirection::Top, title, gui::LayerDirection::Bottom);
        nextButton.setConstraint(gui::LayerDirection::Bottom, view, gui::LayerDirection::Bottom);
        nextButton.setConstraint(gui::LayerDirection::Left);
        nextButton.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right, 0.0, 0.5);
        nextButton.addEventListener(gui::MouseEventType::Down, [this]() {
            guikit->switchPage(U"NextPage");
            });

        switchThemeButton = gui::UIButton(U"Switch theme");
        switchThemeButton.setConstraint(gui::LayerDirection::Top, nextButton, gui::LayerDirection::Top);
        switchThemeButton.setConstraint(gui::LayerDirection::Bottom, nextButton, gui::LayerDirection::Bottom);
        switchThemeButton.setConstraint(gui::LayerDirection::Left, nextButton, gui::LayerDirection::Right);
        switchThemeButton.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);
        switchThemeButton.addEventListener(gui::MouseEventType::Down, [this]() {
            guikit->toggleColorMode();
            });

        view.appendComponent(title);
        view.appendComponent(nextButton);
        view.appendComponent(switchThemeButton);
    }
};

class NextPage : public gui::Page {
    using Page::Page;

    gui::UIButton backButton = gui::UIButton(U"Back");

    void onLoaded() override {
        backButton.setConstraint(gui::LayerDirection::Top);
        backButton.setConstraint(gui::LayerDirection::Bottom, view, gui::LayerDirection::Bottom);
        backButton.setConstraint(gui::LayerDirection::Left);
        backButton.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);
        backButton.addEventListener(gui::MouseEventType::Down, [this]() {
            guikit->switchPage(U"StartPage");
            });

        view.appendComponent(backButton);
    }
};

void Main() {
    gui::GUIKit guikit;
    guikit.appendPage(StartPage(U"StartPage"));
    guikit.appendPage(NextPage(U"NextPage"));
    guikit.start();
}