#include <Aoba/UIButton.hpp>

#include <Aoba/UnifiedFont.hpp>

namespace s3d::aoba {
	ColorTheme UIButton::DefaultColor = DynamicColor::Background;
	ColorTheme UIButton::DefaultTextColor = DynamicColor::Text;
	ColorTheme UIButton::DefaultHoveredColor = DynamicColor::BackgroundSecondary;
	ColorTheme UIButton::DefaultHoveredTextColor = DynamicColor::Text;

	void UIButton::initialize() {
		UIText::initialize();

		setDirection(TextDirection::Center);

		addEventListener<MouseEvent::Hovered>([this] {
			backgroundColor.highlight(hoveredColor);
			textColor.highlight(hoveredTextColor);
			}, true);

		addEventListener<MouseEvent::UnHovered>([this] {
			backgroundColor.lowlight(defaultColor);
			textColor.lowlight(defaultTextColor);
			}, true);

		addEventListener<MouseEvent::Hovering>([] {
			Cursor::RequestStyle(CursorStyle::Hand);
			}, true);
	}

	void UIButton::initializeColors() {
		UIText::initializeColors();

		backgroundColor = defaultColor;
		textColor = defaultTextColor;
	}

	void UIButton::draw() const {
		UIText::draw();

		if (m_icon) {
			m_icon.drawAt(layer().center(), textColor);
		}
	}
}
