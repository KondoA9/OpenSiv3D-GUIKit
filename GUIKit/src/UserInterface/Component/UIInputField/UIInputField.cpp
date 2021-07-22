#include <GUIKit/UIInputField.h>
#include <GUIKit/PixelUnit.h>

using namespace s3d::gui;

void UIInputField::initialize() {
	UIText::initialize();

	addEventListener<MouseEvent::Hovering>([] {
		Cursor::RequestStyle(CursorStyle::IBeam);
		});

	addEventListener<UnFocused>([this] {
		if (!prefix.empty() && !text().starts_with(prefix)) {
			setText(prefix + text());
		}
		if (!suffix.empty() && !text().ends_with(suffix)) {
			setText(text() + suffix);
		}
		}, true);
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
			const auto x = textRegion().right().begin.x + 2_px;
			Line(x, m_fieldRect.y + 4_px, x, m_fieldRect.y + m_fieldRect.h - 4_px).draw(textColor);
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

		bool valid = true;
		for (const auto& word : m_forbiddenWords) {
			if (txt.includes(word)) {
				valid = false;
				break;
			}
		}

		if (valid && pre != txt) {
			setText(txt);
			registerInputEvent(Inputted(this, false));
		}

		if (KeyEnter.down()) {
			registerInputEvent(KeyEnterDown(this, false));
			if (completeInputWhenEnter) {
				unFocus();
			}
		}
	}
}

void UIInputField::updateDrawableText(bool updateField) {
	UIText::updateDrawableText(updateField);

	if (textRegion().h == 0) {
		const auto h = font().fontSize() * 1.416 + 6_px;
		m_fieldRect = RectF(rect().x, textRegion().y - h * 0.5, rect().w, h);
	}
	else {
		m_fieldRect = RectF(rect().x, textRegion().y - 3_px, rect().w, textRegion().h + 6_px);
	}
}
