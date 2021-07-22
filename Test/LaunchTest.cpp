#include <GUIKit/GUIKit.h>

#define ASSERT_MSG(condition, message) assert((condition) && message)

bool loaded = false;
bool beforeAppeared = false;
bool afterAppeared = false;
bool beforeDisappeared = false;
bool afterDisappeared = false;
bool terminated = false;

template<class T>
void LogWithMsg(T* arg, const std::string& msg) {
	std::cout << "[" << __FILE__ << "] " << typeid(*arg).name() << ": " << msg << std::endl;
}

class TestPage1 : public gui::Page {
public:

protected:
	using Page::Page;

	void onLoaded() override {
		// Toggle color mode test
		// This test will ended within 5sec
		gui::GUIKit::Instance().setTimeout([] {
			gui::GUIKit::Instance().setColorMode(gui::ColorMode::Light);
			gui::GUIKit::Instance().toggleColorMode();
			}, 2500, false);

		gui::GUIKit::Instance().setTimeout([this] {
			ASSERT_MSG(
				view().backgroundColor == gui::DynamicColor::Background.dark,
				"Toggled color mode to dark from light, but color of the view is not dark color"
			);
			}, 5000, false);

		// Focus component test
		auto& focustest = gui::GUIFactory::Create<gui::UIRect>();
		view().appendComponent(focustest);
		// This test will ended within 10sec
		{
			const size_t focusError = gui::GUIKit::Instance().setTimeout([] {
				ASSERT_MSG(false, "Focused the component, but event was not triggered");
				}, 5000, false);

			gui::GUIKit::Instance().setTimeout([&focustest] {
				focustest.focus();
				ASSERT_MSG(focustest.isFocused(), "Focused a component, but it is not focused");
				}, 2500, false);

			focustest.addEventListener<gui::Focused>([&focustest, focusError] {
				gui::GUIKit::Instance().stopTimeout(focusError);

				const size_t unfocusError = gui::GUIKit::Instance().setTimeout([] {
					ASSERT_MSG(false, "UnFocused the component, but event was not triggered");
					}, 5000, false);

				gui::GUIKit::Instance().setTimeout([&focustest] {
					focustest.unFocus();
					ASSERT_MSG(!focustest.isFocused(), "UnFocused a component, but it is focused");
					}, 2500, false);

				focustest.addEventListener<gui::UnFocused>([unfocusError] {
					gui::GUIKit::Instance().stopTimeout(unfocusError);
					});
				});
		}

		// 15sec later, switch page to TestPage2
		gui::GUIKit::Instance().setTimeout([] {
			gui::GUIKit::Instance().switchPage(U"TestPage2");
			}, 15000, true);

		loaded = true;

		LogWithMsg(this, "onLoaded");
	}

	void onBeforeAppeared() override {
		beforeAppeared = true;

		LogWithMsg(this, "onBeforeAppeared");
	}

	void onAfterAppeared() override {
		afterAppeared = true;

		LogWithMsg(this, "onAfterAppeared");
	}

	void onBeforeDisappeared() override {
		beforeDisappeared = true;

		LogWithMsg(this, "onBeforeDisappeared");
	}

	void onAfterDisappeared() override {
		afterDisappeared = true;

		LogWithMsg(this, "onAfterDisappeared");
	}

	void onBeforeAppTerminated() override {
		LogWithMsg(this, "onAppTerminated");
	}

	void onAppTerminated() override {
		terminated = true;

		LogWithMsg(this, "onAppTerminated");
	}
};

class TestPage2 : public gui::Page {
	using Page::Page;

	void onLoaded() override;
};

void TestPage2::onLoaded() {
	gui::GUIKit::Instance().setTimeout([] {
		System::Exit();
		}, 10000, true);
}

void Main() {
	auto& guikit = gui::GUIKit::Instance();

	Window::Resize(1280, 720);

	guikit.appendPage<TestPage1>(U"TestPage1");
	guikit.appendPage<TestPage2>(U"TestPage2");

	guikit.start();

	ASSERT_MSG(loaded && beforeAppeared && afterAppeared && beforeDisappeared && afterDisappeared && terminated, "Any paging events were not called.");
}
