#include <Aoba/UIButton.hpp>
#include <Aoba/UnifiedFont.hpp>

namespace s3d::aoba {
	void UIButton::initialize() {
		UIText::initialize();

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
	}

	void UIButton::draw() const {
		UIText::draw();

		if (m_icon) {
			m_icon.drawAt(rect().center(), textColor);
		}
	}
}
