#include <GUIKit/GUIKit.h>

void Terminate(const std::string& msg) {
	std::cerr << msg << std::endl;
	std::terminate();
}

void Assert(bool expression, const std::string& msg) {
	if (!expression) {
		Terminate(msg);
	}
}

std::string LogMessageTemplate() {
	return "[Log]: ";
}

void LogWithMsg(const std::string& msg) {
	std::cout << LogMessageTemplate() << msg << std::endl;
}

template<class T>
void LogWithMsg(T* arg, const std::string& msg) {
	std::cout << LogMessageTemplate() << typeid(*arg).name() << ": " << msg << std::endl;
}

class PageEvent {
	const std::string m_name;
	bool m_value = false;

public:
	PageEvent(const std::string& name) :
		m_name(name)
	{}

	void call() {
		m_value = true;
		LogWithMsg(m_name);
	}

	void validate() const {
		Assert(m_value, LogMessageTemplate() + "Failed: " + m_name);
	}
};

class PagingTest {
	Array<PageEvent> m_events = {
		PageEvent("onLoaded"),
		PageEvent("onBeforeAppeared"),
		PageEvent("onAfterAppeared"),
		PageEvent("onBeforeDisappeared"),
		PageEvent("onAfterDisappeared"),
		PageEvent("onBeforeAppTerminated"),
		PageEvent("onAppTerminated")
	};

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

PagingTest pagingTest;

class LifeCycleTest : public gui::Page {
public:

protected:
	using Page::Page;

	void onLoaded() override {
		// Toggle color mode test
		gui::GUIKit::SetTimeout([] {
			gui::GUIKit::SetColorMode(gui::ColorMode::Light);
			gui::GUIKit::ToggleColorMode();
			}, 2000, false);

		gui::GUIKit::SetTimeout([this] {
			Assert(
				view().backgroundColor == gui::DynamicColor::Background.dark,
				"Toggled color mode to dark from light, but color of the view is not dark color"
			);
			}, 4000, false);

		// 5sec later, switch page
		gui::GUIKit::SetTimeout([] {
			gui::GUIKit::SwitchPage(U"ComponentsTest");
			}, 5000, true);

		pagingTest.loaded();
	}

	void onBeforeAppeared() override {
		pagingTest.beforeAppeared();
	}

	void onAfterAppeared() override {
		pagingTest.afterAppeared();
	}

	void onBeforeDisappeared() override {
		pagingTest.beforeDisappeared();
	}

	void onAfterDisappeared() override {
		pagingTest.afterDisappeared();
	}

	void onBeforeAppTerminated() override {
		pagingTest.beforeAppTerminated();
	}

	void onAppTerminated() override {
		pagingTest.appTerminated();
	}
};

class ComponentsTest : public gui::Page {
	using Page::Page;

	void onLoaded() override;
};

void ComponentsTest::onLoaded() {
	auto& stackView = gui::GUIFactory::Create<gui::UIVStackView>();

	auto& rect = gui::GUIFactory::Create<gui::UIRect>();
	rect.backgroundColor = gui::DynamicColor::DefaultAmber;

	auto& circle = gui::GUIFactory::Create<gui::UICircle>();
	circle.backgroundColor = gui::DynamicColor::DefaultBlue;

	auto& button = gui::GUIFactory::Create<gui::UIButton>();
	button.backgroundColor = gui::DynamicColor::DefaultBlueGray;
	button.setTitle(U"UIButton");

	auto& toggleButton = gui::GUIFactory::Create<gui::UIToggleButton>();
	toggleButton.backgroundColor = gui::DynamicColor::DefaultBrown;
	toggleButton.setTitle(U"UIToggleButton");

	auto& text = gui::GUIFactory::Create<gui::UIText>();
	text.backgroundColor = gui::DynamicColor::DefaultCyan;
	text.setText(U"UIText");

#if !SIV3D_PLATFORM(LINUX)
	auto& inputField = gui::GUIFactory::Create<gui::UIInputField>();
	inputField.backgroundColor = gui::DynamicColor::DefaultDeepOrange;
	inputField.setText(U"UIInputField");
	stackView.appendComponent(inputField);
#endif

	auto& checkBox = gui::GUIFactory::Create<gui::UICheckBox>();
	checkBox.backgroundColor = gui::DynamicColor::DefaultDeepPurple;

	auto& slider = gui::GUIFactory::Create<gui::UISlider>();
	slider.backgroundColor = gui::DynamicColor::DefaultGray;

	auto& icon = gui::GUIFactory::Create<gui::UIIcon>();
	icon.backgroundColor = gui::DynamicColor::DefaultGreen;

	auto& imageView = gui::GUIFactory::Create<gui::UIImageView>();
	imageView.backgroundColor = gui::DynamicColor::DefaultIndigo;

	auto& stackedImageView = gui::GUIFactory::Create<gui::UIZStackedImageView>();
	stackedImageView.backgroundColor = gui::DynamicColor::DefaultLightBlue;

	stackView.appendComponent(rect);
	stackView.appendComponent(circle);
	stackView.appendComponent(button);
	stackView.appendComponent(toggleButton);
	stackView.appendComponent(text);
	stackView.appendComponent(checkBox);
	stackView.appendComponent(slider);
	stackView.appendComponent(icon);
	stackView.appendComponent(imageView);
	stackView.appendComponent(stackedImageView);

	stackView.setConstraint(gui::LayerDirection::Left, view(), gui::LayerDirection::Left);
	stackView.setConstraint(gui::LayerDirection::Top, view(), gui::LayerDirection::Top);
	stackView.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);
	stackView.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);

	view().appendComponent(stackView);

	// 5sec later, switch page
	gui::GUIKit::SetTimeout([] {
		gui::GUIKit::SwitchPage(U"FocusTest");
		}, 5000, true);
}

class FocusTest : public gui::Page {
	using Page::Page;

	void onLoaded() override;
};

void FocusTest::onLoaded() {
	auto& rect = gui::GUIFactory::Create<gui::UIRect>();
	view().appendComponent(rect);

	const size_t focusTimeout = gui::GUIKit::SetTimeout([] {
		Assert(false, "Focused the component, but event was not triggered");
		}, 5000, false);

	gui::GUIKit::SetTimeout([&rect] {
		rect.focus();
		Assert(rect.isFocused(), "Focused a component, but it is not focused");
		}, 2500, false);

	rect.addEventListener<gui::Focused>([&rect, focusTimeout] {
		gui::GUIKit::StopTimeout(focusTimeout);

		const size_t unfocusTimeout = gui::GUIKit::SetTimeout([] {
			Assert(false, "UnFocused the component, but event was not triggered");
			}, 5000, false);

		gui::GUIKit::SetTimeout([&rect] {
			rect.unFocus();
			Assert(!rect.isFocused(), "UnFocused a component, but it is focused");
			}, 2500, false);

		rect.addEventListener<gui::UnFocused>([unfocusTimeout] {
			gui::GUIKit::StopTimeout(unfocusTimeout);
			});
		});

	// 10sec later, terminate app
	gui::GUIKit::SetTimeout([] {
		gui::GUIKit::Terminate();
		}, 10000, true);
}

void Main() {
	Window::Resize(1280, 720);

	gui::GUIKit::AppendPage<LifeCycleTest>(U"LifeCycleTest");
	gui::GUIKit::AppendPage<ComponentsTest>(U"ComponentsTest");
	gui::GUIKit::AppendPage<FocusTest>(U"FocusTest");

	gui::GUIKit::Start();

	pagingTest.validate();
}
