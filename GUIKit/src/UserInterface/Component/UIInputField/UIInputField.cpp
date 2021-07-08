#include <GUIKit/UIInputField.h>

using namespace s3d::gui;

void UIInputField::initialize() {
	UIText::initialize();

	addEventListener<MouseEvent::Hovering>([] {
		Cursor::RequestStyle(CursorStyle::IBeam);
		});
}

void UIInputField::draw() {
	UIText::draw();

	if (isFocused()) {
		m_cursorVisibleTimer += Scene::DeltaTime();
		if (m_cursorVisibleTimer > 0.5) {
			m_cursorVisibleTimer = 0.0;
			m_isCursorVisible = !m_isCursorVisible;
		}

		if (m_isCursorVisible) {
			if (text == U"") {
				Line(m_drawingRect.x, m_drawingRect.y - m_font.fontSize() * 0.5, m_drawingRect.x, m_drawingRect.y + m_font.fontSize() * 0.5).draw(textColor);
			}
			else {
				m_drawingRect.right().draw(textColor);
			}
		}
	}
}

void UIInputField::updateInputEvents() {
	UIText::updateInputEvents();

	if (isFocused()) {
		const String pre = text;
		TextInput::UpdateText(text, TextInputMode::AllowBackSpaceDelete);
		if (pre != text) {
			registerInputEvent(KeyDown(this));
		}
	}
}
