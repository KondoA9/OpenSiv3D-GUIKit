#include <GUIKit/GUIKit.h>

bool loaded = false;
bool beforeAppeared = false;
bool afterAppeared = false;
bool beforeDisappeared = false;
bool afterDisappeared = false;
bool terminated = false;

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
			FMT_ASSERT(
				view().backgroundColor == gui::DynamicColor::Background.dark,
				"Toggled color mode to dark from light, but color of the view is not dark color"
			);
			}, 5000, false);

		// Focus component test
		auto& focustest = gui::GUIFactory::Create<gui::UIRect>();
		view().appendComponent(focustest);
		// This test will ended within 5sec
		{
			size_t focusError = gui::GUIKit::Instance().setTimeout([] {
				FMT_ASSERT(false, "Focused the component, but it is not focused");
				}, 5000, false);

			focustest.addEventListener<gui::Focused>([focusError] {
				gui::GUIKit::Instance().stopTimeout(focusError);
				});

			gui::GUIKit::Instance().setTimeout([&focustest] {
				focustest.focus();
				FMT_ASSERT(focustest.isFocused(), "Focused a component, but it is not focused");
				}, 2500, false);
		}

		// 10sec later, switch page to TestPage2
		gui::GUIKit::Instance().setTimeout([] {
			gui::GUIKit::Instance().switchPage(U"TestPage2");
			}, 10000, true);

		loaded = true;
		std::cout << "[GUIKit Info] Page loaded" << std::endl;
	}

	void onBeforeAppeared() override {
		beforeAppeared = true;
		std::cout << "[GUIKit Info] Page beforeAppeared" << std::endl;
	}

	void onAfterAppeared() override {
		afterAppeared = true;
		std::cout << "[GUIKit Info] Page afterAppeared" << std::endl;
	}

	void onBeforeDisappeared() override {
		beforeDisappeared = true;
		std::cout << "[GUIKit Info] Page beforeDisappeared" << std::endl;
	}

	void onAfterDisappeared() override {
		afterDisappeared = true;
		std::cout << "[GUIKit Info] Page afterDisappeared" << std::endl;
	}

	void onAppTerminated() override {
		terminated = true;
		std::cout << "[GUIKit Info] Page onAppTerminated" << std::endl;
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

	FMT_ASSERT(loaded && beforeAppeared && afterAppeared && beforeDisappeared && afterDisappeared && terminated, "Any paging events were not called.");
}
