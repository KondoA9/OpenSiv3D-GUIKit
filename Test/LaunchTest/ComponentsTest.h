class ComponentsTest : public gui::Page {
	using Page::Page;

	void onLoaded() override;
};

void ComponentsTest::onLoaded() {
	auto& stackView = gui::GUIFactory::Create<gui::UIVStackView>();
	stackView.setConstraint(gui::LayerDirection::Top, view(), gui::LayerDirection::Top);
	stackView.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
	stackView.setConstraint(gui::LayerDirection::Left, view(), gui::LayerDirection::Left);
	stackView.setConstraint(gui::LayerDirection::Width, view(), gui::LayerDirection::Width, 0.0, 0.5);
	view().appendComponent(stackView);

	auto& tabView = gui::GUIFactory::Create<gui::UITabView>();
	tabView.setConstraint(gui::LayerDirection::Top, view(), gui::LayerDirection::Top);
	tabView.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
	tabView.setConstraint(gui::LayerDirection::Left, stackView, gui::LayerDirection::Right);
	tabView.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);
	view().appendComponent(tabView);

	auto& rect = gui::GUIFactory::Create<gui::UIRect>();
	rect.backgroundColor = gui::DynamicColor::DefaultAmber;
	stackView.appendComponent(rect);

	auto& circle = gui::GUIFactory::Create<gui::UICircle>();
	circle.backgroundColor = gui::DynamicColor::DefaultBlue;
	stackView.appendComponent(circle);

	auto& button = gui::GUIFactory::Create<gui::UIButton>();
	button.backgroundColor = gui::DynamicColor::DefaultBlueGray;
	button.setText(U"UIButton");
	stackView.appendComponent(button);

	auto& toggleButton = gui::GUIFactory::Create<gui::UIToggleButton>();
	toggleButton.backgroundColor = gui::DynamicColor::DefaultBrown;
	toggleButton.setText(U"UIToggleButton");
	stackView.appendComponent(toggleButton);

	auto& text = gui::GUIFactory::Create<gui::UIText>();
	text.backgroundColor = gui::DynamicColor::DefaultCyan;
	text.setText(U"UIText");
	stackView.appendComponent(text);

#if !SIV3D_PLATFORM(LINUX)
	auto& inputField = gui::GUIFactory::Create<gui::UIInputField>();
	inputField.backgroundColor = gui::DynamicColor::DefaultDeepOrange;
	inputField.setText(U"UIInputField");
	stackView.appendComponent(inputField);
#endif

	auto& checkBox = gui::GUIFactory::Create<gui::UICheckBox>();
	checkBox.backgroundColor = gui::DynamicColor::DefaultDeepPurple;
	checkBox.setChecked(true);
	stackView.appendComponent(checkBox);

	auto& slider = gui::GUIFactory::Create<gui::UISlider>();
	slider.backgroundColor = gui::DynamicColor::DefaultGray;
	stackView.appendComponent(slider);

	auto& icon = gui::GUIFactory::Create<gui::UIIcon>();
	icon.backgroundColor = gui::DynamicColor::DefaultGreen;
	icon.setIcon(Icon(0xf109), 40_px);
	stackView.appendComponent(icon);

	auto& imageView = gui::GUIFactory::Create<gui::UIImageView>();
	imageView.backgroundColor = gui::DynamicColor::DefaultIndigo;
	imageView.appendImage(Image(U"asset/image1.png"));
	stackView.appendComponent(imageView);

	auto& tab1 = gui::GUIFactory::Create<gui::UIView>();
	tab1.backgroundColor = gui::DynamicColor::DefaultBlue;
	tabView.appendTab(U"Tab1", tab1);

	auto& tab2 = gui::GUIFactory::Create<gui::UIView>();
	tab2.backgroundColor = gui::DynamicColor::DefaultGreen;
	tabView.appendTab(U"Tab2", tab2);

	// 5sec later, switch page
	gui::GUIKit::SetTimeout([] {
		gui::GUIKit::SwitchPage(U"FocusTest");
		}, 5000, true);
}
