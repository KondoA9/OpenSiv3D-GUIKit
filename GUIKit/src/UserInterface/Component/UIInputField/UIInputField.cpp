#include <GUIKit/UIInputField.h>
#include <GUIKit/PixelUnit.h>

using namespace s3d::gui;

void UIInputField::initialize() {
	UIText::initialize();

	addEventListener<MouseEvent::Hovering>([] {
		Cursor::RequestStyle(CursorStyle::IBeam);
		});
}

void UIInputField::draw() {
	m_fieldRect.draw(DynamicColor::BackgroundSecondary);

	UIText::draw();

	if (isFocused()) {
		m_cursorVisibleTimer += Scene::DeltaTime();
		if (m_cursorVisibleTimer > 0.5) {
			m_cursorVisibleTimer = 0.0;
			m_isCursorVisible = !m_isCursorVisible;
		}

		if (m_isCursorVisible) {
			textRegion().right().moveBy(2_px, 0.0).draw(textColor);
		}

		m_fieldRect.drawFrame(1.0_px, 0.0, DynamicColor::DefaultBlue);
	}
}

void UIInputField::updateInputEvents() {
	UIText::updateInputEvents();

	if (isFocused()) {
		const String pre = text();
		String txt = text();
		TextInput::UpdateText(txt, TextInputMode::AllowBackSpaceDelete);
		setText(txt);
		if (pre != text()) {
			updateDrawableText();
			registerInputEvent(KeyDown(this));
		}
	}
}

void UIInputField::updateDrawableText() {
	UIText::updateDrawableText();

	if (textRegion().h == 0) {
		const auto h = font().fontSize() + 6_px;
		m_fieldRect = RectF(m_rect.x, textRegion().y - h * 0.5, m_rect.w, h);
	}
	else {
		m_fieldRect = RectF(m_rect.x, textRegion().y - 3_px, m_rect.w, textRegion().h + 6_px);
	}
}
