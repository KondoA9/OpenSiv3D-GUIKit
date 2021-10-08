class ComponentsTest : public aoba::Page {
	using Page::Page;

	void onLoaded() override;
};

void ComponentsTest::onLoaded() {
	auto& stackView = aoba::AobaFactory::Create<aoba::UIVStackView>(view);
	stackView.setConstraint(aoba::LayerDirection::Top, view, aoba::LayerDirection::Top);
	stackView.setConstraint(aoba::LayerDirection::Bottom, view, aoba::LayerDirection::Bottom);
	stackView.setConstraint(aoba::LayerDirection::Left, view, aoba::LayerDirection::Left);
	stackView.setConstraint(aoba::LayerDirection::Width, view, aoba::LayerDirection::Width, 0.0, 0.5);

	auto& tabView = aoba::AobaFactory::Create<aoba::UITabView>(view);
	tabView.setConstraint(aoba::LayerDirection::Top, view, aoba::LayerDirection::Top);
	tabView.setConstraint(aoba::LayerDirection::Bottom, view, aoba::LayerDirection::Bottom);
	tabView.setConstraint(aoba::LayerDirection::Left, stackView, aoba::LayerDirection::Right);
	tabView.setConstraint(aoba::LayerDirection::Right, view, aoba::LayerDirection::Right);

	auto& rect = aoba::AobaFactory::Create<aoba::UIRect>(stackView);
	rect.backgroundColor = aoba::DynamicColor::DefaultAmber;

	auto& circle = aoba::AobaFactory::Create<aoba::UICircle>(stackView);
	circle.backgroundColor = aoba::DynamicColor::DefaultBlue;

	auto& button = aoba::AobaFactory::Create<aoba::UIButton>(stackView);
	button.backgroundColor = aoba::DynamicColor::DefaultBlueGray;
	button.setText(U"UIButton");

	auto& toggleButton = aoba::AobaFactory::Create<aoba::UIToggleButton>(stackView);
	toggleButton.backgroundColor = aoba::DynamicColor::DefaultBrown;
	toggleButton.setText(U"UIToggleButton");

	auto& text = aoba::AobaFactory::Create<aoba::UIText>(stackView);
	text.backgroundColor = aoba::DynamicColor::DefaultCyan;
	text.setText(U"UIText");

#if !SIV3D_PLATFORM(LINUX)
	auto& inputField = aoba::AobaFactory::Create<aoba::UIInputField>(stackView);
	inputField.backgroundColor = aoba::DynamicColor::DefaultDeepOrange;
	inputField.setText(U"UIInputField");
#endif

	auto& checkBox = aoba::AobaFactory::Create<aoba::UICheckBox>(stackView);
	checkBox.backgroundColor = aoba::DynamicColor::DefaultDeepPurple;
	checkBox.setChecked(true);

	auto& slider = aoba::AobaFactory::Create<aoba::UISlider>(stackView);
	slider.backgroundColor = aoba::DynamicColor::DefaultGray;

	auto& icon = aoba::AobaFactory::Create<aoba::UIIcon>(stackView);
	icon.backgroundColor = aoba::DynamicColor::DefaultGreen;
	icon.setIcon(Icon(0xf109), 40_px);

	auto& imageView = aoba::AobaFactory::Create<aoba::UIImageView>(stackView);
	imageView.backgroundColor = aoba::DynamicColor::DefaultIndigo;
	imageView.appendImage(Image(U"asset/image1.png"));

	auto& tab1 = tabView.appendTab<aoba::UIView>(U"Tab1");
	tab1.backgroundColor = aoba::DynamicColor::DefaultBlue;

	auto& tab2 = tabView.appendTab<aoba::UIView>(U"Tab2");
	tab2.backgroundColor = aoba::DynamicColor::DefaultBlue;

	// 5sec later, switch page
	aoba::SetTimeout([] {
		aoba::SwitchPage(U"FocusTest");
		}, 5000, true);
}
