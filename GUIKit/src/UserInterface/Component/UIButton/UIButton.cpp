#include <GUIKit/UIButton.h>
#include <GUIKit/UnifiedFont.h>

using namespace s3d::gui;

void UIButton::initialize() {
	backgroundColor = defaultColor;

	addEventListener<MouseEvent::Hovered>([this] {
		backgroundColor.highlight(hoveredColor);
		}, true);

	addEventListener<MouseEvent::UnHovered>([this] {
		backgroundColor.lowlight(defaultColor);
		}, true);

	addEventListener<MouseEvent::Hovering>([] {
		Cursor::RequestStyle(CursorStyle::Hand);
		}, true);

	UIRect::initialize();
}

void UIButton::draw() {
	UIRect::draw();

	UnifiedFont::Get(UnifiedFontStyle::Medium)(m_title).drawAt(rect().center(), textColor);

	if (m_icon) {
		m_icon.drawAt(rect().center(), textColor);
	}
}
