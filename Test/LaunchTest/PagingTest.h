class PageEvent {
	const std::string m_name;
	bool m_value = false;

public:
	PageEvent(const std::string& name) :
		m_name(name)
	{}

	void call() {
		m_value = true;
		Test::LogWithMsg(m_name);
	}

	void validate() const {
		Test::Assert(m_value, Test::LogMessageTemplate() + "Failed: " + m_name);
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

class LifeCycleTest : public aoba::Page {
public:

protected:
	using Page::Page;

	void onLoaded() override {
		// Toggle color mode test
		aoba::Core::SetTimeout([] {
			aoba::Core::SetColorMode(aoba::ColorMode::Light);
			aoba::Core::ToggleColorMode();
			}, 2000, false);

		aoba::Core::SetTimeout([this] {
			Test::Assert(
				view.backgroundColor == aoba::DynamicColor::Background.dark,
				"Toggled color mode to dark from light, but color of the view is not dark color"
			);
			}, 4000, false);

		// 5sec later, switch page
		aoba::Core::SetTimeout([] {
			aoba::Core::SwitchPage(U"ComponentsTest");
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
