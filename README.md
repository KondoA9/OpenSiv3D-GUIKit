# OpenSiv3D GUIKit
C++ GUI framework with [**OpenSiv3D v0.6.1**](https://zenn.dev/reputeless/books/siv3d-documentation).

**:construction: This repository is under development.**

## Features
 - Comfortable user interface
 - Light / Dark theme
 - Automatic layout system
 - Event-driven programming
 
## Usage
- See following workflows. 
	- [Windows](https://github.com/KondoA9/OpenSiv3D-GUIKit/blob/695b63c090c24957de28e41c580685959084e811/.github/workflows/ci.yml#L22-L31)
	- [macOS](https://github.com/KondoA9/OpenSiv3D-GUIKit/blob/695b63c090c24957de28e41c580685959084e811/.github/workflows/ci.yml#L41-L48)
	- [Linux](https://github.com/KondoA9/OpenSiv3D-GUIKit/blob/695b63c090c24957de28e41c580685959084e811/.github/workflows/ci.yml#L62-L114)
- This repository includes static library of OpenSiv3D. 

## Example
 - [Example Code](https://github.com/KondoA9/OpenSiv3D-GUIKit/blob/develop/Test/Example/Example.cpp)
 - [Test Code](https://github.com/KondoA9/OpenSiv3D-GUIKit/tree/develop/Test/LaunchTest)

**:warning: The following code does not use the latest version of GUIKit.**

 ```c++
#include <GUIKit/GUIKit.h>

class StartPage : public gui::Page {
	using Page::Page;

	gui::UIText& title = gui::GUIFactory::Create<gui::UIText>();
	gui::UIButton& nextButton = gui::GUIFactory::Create<gui::UIButton>();
	gui::UIButton& switchThemeButton = gui::GUIFactory::Create<gui::UIButton>();
	gui::UIVStackView& scrollArea = gui::GUIFactory::Create<gui::UIVStackView>();

	void onLoaded() override {
		title.setText(U"GUIKit");
		title.setFont(gui::UnifiedFontStyle::Large);
		title.setDirection(gui::TextDirection::Center);

		nextButton.setTitle(U"Next");

		switchThemeButton.setTitle(U"Switch theme");

		title.setConstraint(gui::LayerDirection::Top);
		title.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom, 0.0, 0.5);
		title.setConstraint(gui::LayerDirection::Left);
		title.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);

		nextButton.setConstraint(gui::LayerDirection::Top, title, gui::LayerDirection::Bottom);
		nextButton.setConstraint(gui::LayerDirection::Height, view(), gui::LayerDirection::Height, 0.0, 0.25);
		nextButton.setConstraint(gui::LayerDirection::Left);
		nextButton.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right, 0.0, 0.5);
		nextButton.addEventListener<gui::MouseEvent::LeftDown>([this]() {
			gui::GUIKit::SwitchPage(U"next");
			});

		switchThemeButton.setConstraint(gui::LayerDirection::Top, nextButton, gui::LayerDirection::Bottom);
		switchThemeButton.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
		switchThemeButton.setConstraint(gui::LayerDirection::Left, view(), gui::LayerDirection::Left);
		switchThemeButton.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right, 0.0, 0.5);
		switchThemeButton.addEventListener<gui::MouseEvent::LeftDown>([this]() {
			gui::GUIKit::ToggleColorMode();
			});

		scrollArea.setRowHeight(40_px);
		scrollArea.scrollingEnabled = true;
		scrollArea.backgroundColor = gui::DynamicColor::BackgroundSecondary;
		scrollArea.setConstraint(gui::LayerDirection::Top, title, gui::LayerDirection::Bottom);
		scrollArea.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
		scrollArea.setConstraint(gui::LayerDirection::Left, nextButton, gui::LayerDirection::Right);
		scrollArea.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);
		scrollArea.addEventListener<gui::MouseEvent::LeftDown>([this]() {
			gui::GUIKit::ToggleColorMode();
			});

		for (size_t i = 0; i < 20; i++) {
			auto& button = gui::GUIFactory::Create<gui::UIButton>();
			button.setTitle(ToString(i));
			button.penetrateMouseEvent = true;
			scrollArea.appendComponent(button);
		}

		view().appendComponent(title);
		view().appendComponent(nextButton);
		view().appendComponent(switchThemeButton);
		view().appendComponent(scrollArea);
	}
};

class NextPage : public gui::Page {
	using Page::Page;

	gui::UIButton& backButton = gui::GUIFactory::Create<gui::UIButton>();

	void onLoaded() override {
		backButton.setTitle(U"Back");

		backButton.setConstraint(gui::LayerDirection::Top);
		backButton.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
		backButton.setConstraint(gui::LayerDirection::Left);
		backButton.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);
		backButton.addEventListener<gui::MouseEvent::LeftDown>([this]() {
			gui::GUIKit::SwitchPage(U"start");
			});

		view().appendComponent(backButton);
	}
};

void Main() {
	gui::GUIKit::AppendPage<StartPage>(U"start");
	gui::GUIKit::AppendPage<NextPage>(U"next");
	gui::GUIKit::Start();
}
 ```

## Create custom input event
- Define a struct that inherits InputEvent struct by using GUICreateInputEvent() macro
- Override and fix UIComponent::updateInputEvents() as like following code

```c++
GUICreateInputEvent(CustomEvent);

class UICustomRect: public gui::UIRect {
protected:
    void updateInputEvents() override {
        UIRect::updateInputEvents();

        if(custom_event_callable){
            registerInputEvent(CustomEvent(this));
        }
    }
};

// Using
auto& rect = gui::GUIFactory::Create<UICustomRect>();
rect.addEventListener<CustomEvent>([]() {
    // If custom_event_callable, then this code runs
    });

// You can use CustomEvent
rect.addEventListener<CustomEvent>([](const CustomEvent& e) {
    size_t event_type_id = e.id;
    double wheel = e.wheel;
    Vec2 mousePos = e.pos;
    Vec2 previousMousePos = e.previousPos;
    UICustomRect* component = static_cast<UICustomRect*>(e.component);
    });
```