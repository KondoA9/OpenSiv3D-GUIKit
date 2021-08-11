#include <GUIKit/UIButton.hpp>
#include <GUIKit/UnifiedFont.hpp>

namespace s3d::gui {
	void UIButton::initialize() {
		backgroundColor = defaultColor;
		textColor = defaultTextColor;

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

		UIText::initialize();
	}

	void UIButton::draw() {
		UIText::draw();

		if (m_icon) {
			m_icon.drawAt(rect().center(), textColor);
		}
	}
}
