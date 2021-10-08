class FocusTest : public aoba::Page {
	using Page::Page;

	void onLoaded() override;
};

void FocusTest::onLoaded() {
	auto& rect = aoba::AobaFactory::Create<aoba::UIRect>(view);

	const size_t focusTimeout = aoba::SetTimeout([] {
		Test::Assert(false, "Focused the component, but event was not triggered");
		}, 3000, false);

	aoba::SetTimeout([&rect] {
		rect.focus();
		Test::Assert(rect.isFocused(), "Focused a component, but it is not focused");
		}, 1500, false);

	rect.addEventListener<aoba::Focused>([&rect, focusTimeout] {
		aoba::StopTimeout(focusTimeout);

		const size_t unfocusTimeout = aoba::SetTimeout([] {
			Test::Assert(false, "UnFocused the component, but event was not triggered");
			}, 3000, false);

		aoba::SetTimeout([&rect] {
			rect.unFocus();
			Test::Assert(!rect.isFocused(), "UnFocused a component, but it is focused");
			}, 1500, false);

		rect.addEventListener<aoba::UnFocused>([unfocusTimeout] {
			aoba::StopTimeout(unfocusTimeout);
			});
		});

	// 5sec later, terminate app
	aoba::SetTimeout([] {
		aoba::Terminate();
		}, 5000, true);
}
