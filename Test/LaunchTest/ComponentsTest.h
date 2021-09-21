class ComponentsTest : public gui::Page {
	using Page::Page;

	void onLoaded() override;
};

void ComponentsTest::onLoaded() {
	auto& stackView = gui::GUIFactory::Create<gui::UIVStackView>(view());
	stackView.setConstraint(gui::LayerDirection::Top, view(), gui::LayerDirection::Top);
	stackView.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
	stackView.setConstraint(gui::LayerDirection::Left, view(), gui::LayerDirection::Left);
	stackView.setConstraint(gui::LayerDirection::Width, view(), gui::LayerDirection::Width, 0.0, 0.5);

	auto& tabView = gui::GUIFactory::Create<gui::UITabView>(view());
	tabView.setConstraint(gui::LayerDirection::Top, view(), gui::LayerDirection::Top);
	tabView.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
	tabView.setConstraint(gui::LayerDirection::Left, stackView, gui::LayerDirection::Right);
	tabView.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);

	auto& rect = gui::GUIFactory::Create<gui::UIRect>(stackView);
	rect.backgroundColor = gui::DynamicColor::DefaultAmber;

	auto& circle = gui::GUIFactory::Create<gui::UICircle>(stackView);
	circle.backgroundColor = gui::DynamicColor::DefaultBlue;

	auto& button = gui::GUIFactory::Create<gui::UIButton>(stackView);
	button.backgroundColor = gui::DynamicColor::DefaultBlueGray;
	button.setText(U"UIButton");

	auto& toggleButton = gui::GUIFactory::Create<gui::UIToggleButton>(stackView);
	toggleButton.backgroundColor = gui::DynamicColor::DefaultBrown;
	toggleButton.setText(U"UIToggleButton");

	auto& text = gui::GUIFactory::Create<gui::UIText>(stackView);
	text.backgroundColor = gui::DynamicColor::DefaultCyan;
	text.setText(U"UIText");

#if !SIV3D_PLATFORM(LINUX)
	auto& inputField = gui::GUIFactory::Create<gui::UIInputField>(stackView);
	inputField.backgroundColor = gui::DynamicColor::DefaultDeepOrange;
	inputField.setText(U"UIInputField");
#endif

	auto& checkBox = gui::GUIFactory::Create<gui::UICheckBox>(stackView);
	checkBox.backgroundColor = gui::DynamicColor::DefaultDeepPurple;
	checkBox.setChecked(true);

	auto& slider = gui::GUIFactory::Create<gui::UISlider>(stackView);
	slider.backgroundColor = gui::DynamicColor::DefaultGray;

	auto& icon = gui::GUIFactory::Create<gui::UIIcon>(stackView);
	icon.backgroundColor = gui::DynamicColor::DefaultGreen;
	icon.setIcon(Icon(0xf109), 40_px);

	auto& imageView = gui::GUIFactory::Create<gui::UIImageView>(stackView);
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
