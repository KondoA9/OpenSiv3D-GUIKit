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

class TestPage1 : public Page {
public:

protected:
	using Page::Page;

	void onLoaded() override {
		// Toggle color mode test
		// This test will ended within 5sec
		GUIKit::SetTimeout([] {
			GUIKit::SetColorMode(ColorMode::Light);
			GUIKit::ToggleColorMode();
			}, 2500, false);

		GUIKit::SetTimeout([this] {
			Assert(
				view().backgroundColor == DynamicColor::Background.dark,
				"Toggled color mode to dark from light, but color of the view is not dark color"
			);
			}, 5000, false);

		// Focus component test
		auto& focustest = GUIFactory::Create<UIRect>();
		view().appendComponent(focustest);
		// This test will ended within 10sec
		{
			const size_t focusError = GUIKit::SetTimeout([] {
				Assert(false, "Focused the component, but event was not triggered");
				}, 5000, false);

			GUIKit::SetTimeout([&focustest] {
				focustest.focus();
				Assert(focustest.isFocused(), "Focused a component, but it is not focused");
				}, 2500, false);

			focustest.addEventListener<Focused>([&focustest, focusError] {
				GUIKit::StopTimeout(focusError);

				const size_t unfocusError = GUIKit::SetTimeout([] {
					Assert(false, "UnFocused the component, but event was not triggered");
					}, 5000, false);

				GUIKit::SetTimeout([&focustest] {
					focustest.unFocus();
					Assert(!focustest.isFocused(), "UnFocused a component, but it is focused");
					}, 2500, false);

				focustest.addEventListener<UnFocused>([unfocusError] {
					GUIKit::StopTimeout(unfocusError);
					});
				});
		}

		// 15sec later, switch page to TestPage2
		GUIKit::SetTimeout([] {
			GUIKit::SwitchPage(U"TestPage2");
			}, 15000, true);


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

class TestPage2 : public Page {
	using Page::Page;

	void onLoaded() override;
};

void TestPage2::onLoaded() {
	GUIKit::SetTimeout([] {
		GUIKit::Terminate();
		}, 10000, true);
}

void Main() {
	Window::Resize(1280, 720);

	GUIKit::AppendPage<TestPage1>(U"TestPage1");
	GUIKit::AppendPage<TestPage2>(U"TestPage2");

	GUIKit::Start();

	pagingTest.validate();
}
