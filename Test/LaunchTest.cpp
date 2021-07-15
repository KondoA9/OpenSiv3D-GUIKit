#include <GUIKit/GUIKit.h>

class TestPage : public gui::Page {
	using Page::Page;

	void onLoaded() override {
		Logger << U"Loaded";
	}

	void onBeforeAppeared() override {
		Logger << U"Will be appeared";
	}

	void onAfterAppeared() override {
		Logger << U"Appeared";

		gui::GUIKit::Instance().setTimeout([]{
			System::Exit();
		}, 5000, true);
	}

	void onBeforeDisappeared() override {
		Logger << U"Will be disappeared";
	}

	void onAfterDisappeared() override {
		Logger << U"Disappeared";
	}

	void onAppTerminated() override {
		Logger << U"Terminated";
	}
};

void Main() {
	auto& guikit = gui::GUIKit::Instance();

	Window::Resize(1280, 720);

	guikit.appendPage<TestPage>(U"LaunchTest");

	guikit.start();
}
