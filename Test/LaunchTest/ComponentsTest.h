class ComponentsTest : public gui::Page {
	using Page::Page;

	void onLoaded() override;
};

void ComponentsTest::onLoaded() {
	auto& stackView = view().createComponent<gui::UIVStackView>();
	stackView.setConstraint(gui::LayerDirection::Top, view(), gui::LayerDirection::Top);
	stackView.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
	stackView.setConstraint(gui::LayerDirection::Left, view(), gui::LayerDirection::Left);
	stackView.setConstraint(gui::LayerDirection::Width, view(), gui::LayerDirection::Width, 0.0, 0.5);

	auto& tabView = view().createComponent<gui::UITabView>();
	tabView.setConstraint(gui::LayerDirection::Top, view(), gui::LayerDirection::Top);
	tabView.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
	tabView.setConstraint(gui::LayerDirection::Left, stackView, gui::LayerDirection::Right);
	tabView.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);

	auto& rect = stackView.createComponent<gui::UIRect>();
	rect.backgroundColor = gui::DynamicColor::DefaultAmber;

	auto& circle = stackView.createComponent<gui::UICircle>();
	circle.backgroundColor = gui::DynamicColor::DefaultBlue;

	auto& button = stackView.createComponent<gui::UIButton>();
	button.backgroundColor = gui::DynamicColor::DefaultBlueGray;
	button.setText(U"UIButton");

	auto& toggleButton = stackView.createComponent<gui::UIToggleButton>();
	toggleButton.backgroundColor = gui::DynamicColor::DefaultBrown;
	toggleButton.setText(U"UIToggleButton");

	auto& text = stackView.createComponent<gui::UIText>();
	text.backgroundColor = gui::DynamicColor::DefaultCyan;
	text.setText(U"UIText");

#if !SIV3D_PLATFORM(LINUX)
	auto& inputField = stackView.createComponent<gui::UIInputField>();
	inputField.backgroundColor = gui::DynamicColor::DefaultDeepOrange;
	inputField.setText(U"UIInputField");
#endif

	auto& checkBox = stackView.createComponent<gui::UICheckBox>();
	checkBox.backgroundColor = gui::DynamicColor::DefaultDeepPurple;
	checkBox.setChecked(true);

	auto& slider = stackView.createComponent<gui::UISlider>();
	slider.backgroundColor = gui::DynamicColor::DefaultGray;

	auto& icon = stackView.createComponent<gui::UIIcon>();
	icon.backgroundColor = gui::DynamicColor::DefaultGreen;
	icon.setIcon(Icon(0xf109), 40_px);

	auto& imageView = stackView.createComponent<gui::UIImageView>();
	imageView.backgroundColor = gui::DynamicColor::DefaultIndigo;
	imageView.appendImage(Image(U"asset/image1.png"));

	auto& tab1 = tabView.appendTab<gui::UIView>(U"Tab1");
	tab1.backgroundColor = gui::DynamicColor::DefaultBlue;

	auto& tab2 = tabView.appendTab<gui::UIView>(U"Tab2");
	tab2.backgroundColor = gui::DynamicColor::DefaultBlue;

	// 5sec later, switch page
	gui::GUIKit::SetTimeout([] {
		gui::GUIKit::SwitchPage(U"FocusTest");
		}, 5000, true);
}
