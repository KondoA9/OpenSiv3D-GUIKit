# OpenSiv3D GUIKit
C++ GUI framework using [OpenSiv3D](https://siv3d.github.io/ja-jp/).

## Features
 - Comfortable user interface
 - Light / Dark theme
 - Automatic layout system
 - Event-driven programming
 
 ## Usage
  - Include GUIKit (.cpp&.h) into your project.
  - Build GUIKit and use a static library.

 ## Example
 ```c++
#include <GUIKit/GUIKit.h>

class StartPage : public gui::Page {
    using Page::Page;

    gui::UIText title = gui::UIText(U"GUIKit", gui::UnifiedFontStyle::Large, gui::TextDirection::Center);
    gui::UIButton nextButton = gui::UIButton(U"Next");
    gui::UIButton switchThemeButton = gui::UIButton(U"Switch theme");

    void onLoaded() override {
        title.setConstraint(gui::LayerDirection::Top);
        title.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom, 0.0, 0.5);
        title.setConstraint(gui::LayerDirection::Left);
        title.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);

        nextButton.setConstraint(gui::LayerDirection::Top, title, gui::LayerDirection::Bottom);
        nextButton.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom);
        nextButton.setConstraint(gui::LayerDirection::Left);
        nextButton.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right, 0.0, 0.5);
        nextButton.addEventListener<gui::MouseEvent::LeftDown>([this]() {
            guikit().switchPage(U"next");
            });

        switchThemeButton.setConstraint(gui::LayerDirection::Top, nextButton, gui::LayerDirection::Top);
        switchThemeButton.setConstraint(gui::LayerDirection::Bottom, nextButton, gui::LayerDirection::Bottom);
        switchThemeButton.setConstraint(gui::LayerDirection::Left, nextButton, gui::LayerDirection::Right);
        switchThemeButton.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);
        switchThemeButton.addEventListener<gui::MouseEvent::LeftDown>([this]() {
            guikit().toggleColorMode();
            });

        m_view.appendComponent(title);
        m_view.appendComponent(nextButton);
        m_view.appendComponent(switchThemeButton);
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
        backButton.addEventListener<gui::MouseEvent::LeftDown>([this]() {
            guikit().switchPage(U"start");
            });

        m_view.appendComponent(backButton);
    }
};

void Main() {
    gui::GUIKit guikit;
    guikit.appendPage(StartPage(U"start"));
    guikit.appendPage(NextPage(U"next"));
    guikit.start();
}
 ```

## Create custom input event
- Define a struct that inherits InputEvent struct by using GUICreateInputEvent() macro
- Override and fix UIComponent::updateInputEvents() as like following code

```c++
GUICreateInputEvent(CustomEvent);

class UICustomRect: public UIRect {
protected:
    void updateInputEvents() override {
        UIRect::updateInputEvents();

        if(custom_event_callable){
            callInputEventHandler(CustomEvent(this));
        }
    }
}

// Using
auto rect = UICustomRect();
rect.addEventListener<CustomEvent>([]() {
    // If custom_event_callable, then this code runs
    });

// You can use CustomEvent
rect.addEventListener<CustomEvent>([](CustomEvent e) {
    size_t event_type_id = e.id;
    double wheel = e.wheel;
    Vec2 mousePos = e.pos;
    Vec2 previousMousePos = e.previousPos;
    UICustomRect* component = static_cast<UICustomRect*>(e.component);
    });
```