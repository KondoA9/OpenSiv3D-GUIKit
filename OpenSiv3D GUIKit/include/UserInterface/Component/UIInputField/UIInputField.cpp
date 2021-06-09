#include "UIInputField.h"

using namespace s3d::gui;

void UIInputField::draw() {
	if (m_focused) {
		const String pre = text;
		TextInput::UpdateText(text);
		if (pre != text && m_onInputtedHandler) {
			m_onInputtedHandler(text);
		}
	}

	UIText::draw();

	if (m_focused) {
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

bool UIInputField::mouseLeftDown() {
	if (UIRect::mouseLeftDown()) {
		m_focused = true;
		return true;
	}

	return false;
}