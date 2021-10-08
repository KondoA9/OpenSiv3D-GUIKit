#include <Aoba/UIToggleButton.hpp>

namespace s3d::aoba {
	void UIToggleButton::initialize() {
		UIText::initialize();

		backgroundColor = defaultColor;
		textColor = defaultTextColor;

		addEventListener<MouseEvent::LeftDown>([this] {
			m_enabled = !m_enabled;
			if (m_enabled) {
				backgroundColor.highlight(selectedColor);
				textColor.highlight(selectedTextColor);
			}
			else {
				backgroundColor.lowlight(hoveredColor);
				textColor.lowlight(defaultTextColor);
			}
			}, true);

		addEventListener<MouseEvent::Hovered>([this] {
			if (!m_enabled) {
				backgroundColor.highlight(hoveredColor);
			}
			else {
				backgroundColor.highlight(selectedColor);
			}
			}, true);

		addEventListener<MouseEvent::UnHovered>([this] {
			if (!m_enabled) {
				backgroundColor.lowlight(defaultColor);
			}
			else {
				backgroundColor.highlight(selectedColor);
			}
			}, true);

		addEventListener<MouseEvent::Hovering>([] {
			Cursor::RequestStyle(CursorStyle::Hand);
			}, true);
	}
}
