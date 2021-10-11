#include <Aoba/UIInputField.hpp>
#include <Aoba/PixelUnit.hpp>
#include <Aoba/AobaFactory.hpp>
#include <Aoba/DynamicColor.hpp>

namespace s3d::aoba {
	const Array<char32> UIInputField::ForbiddenPathChar = { '\\', '/', ':', '*', '*', '?', '<', '>', '|' };
	const Array<char32> UIInputField::ForbiddenPathCharRecommended = { ' ', ';', ',' };
	UIText* UIInputField::ui_Warning = nullptr;
	size_t UIInputField::m_WarningTimeoutID = 0;

	void UIInputField::initialize() {
		UIText::initialize();

		addEventListener<MouseEvent::Hovering>([] {
			Cursor::RequestStyle(CursorStyle::IBeam);
			});

		addEventListener<Focused>([this] {
			m_cursorPos = text().length();
			m_cursorBeamWatcher.start();
			}), true;

		addEventListener<UnFocused>([this] {
			if (!prefix.empty() && !text().starts_with(prefix)) {
				setText(prefix + text());
			}
			if (!suffix.empty() && !text().ends_with(suffix)) {
				setText(text() + suffix);
			}

			m_cursorBeamWatcher.reset();
			}, true);
	}

	void UIInputField::update() {
		UIText::update();

		if (isFocused()) {
			if (m_cursorBeamWatcher.ms() > 500) {
				m_cursorBeamWatcher.restart();
				m_isCursorVisible = !m_isCursorVisible;
			}

			updateCursorMovement();

			updateCursorBeamPos();
		}
	}

	void UIInputField::draw() const {
		m_fieldRect.draw(DynamicColor::BackgroundSecondary);

		UIText::draw();

		if (isFocused()) {
			if (m_isCursorVisible) {
				Line(m_cursorBeamPosX, m_fieldRect.y + 4_px, m_cursorBeamPosX, m_fieldRect.y + m_fieldRect.h - 4_px).draw(textColor);
			}

			m_fieldRect.drawFrame(1.0_px, 0.0, DynamicColor::DefaultBlue);
		}
	}

	void UIInputField::updateInputEvents() {
		UIText::updateInputEvents();

		if (isFocused()) {
			const String previousText = text();

			const String updatedText = updateText();

			if (previousText != updatedText) {
				setText(updatedText);
				registerInputEvent(Inputted(this, false));
			}

			if (KeyEnter.down()) {
				registerInputEvent(KeyEnterDown(this, false));
				unFocus();
			}
		}
	}

	void UIInputField::updateDrawableText(bool updateField) {
		UIText::updateDrawableText(updateField);

		if (textRegion().h == 0) {
			const auto h = font().fontSize() * 1.416 + 6_px;
			m_fieldRect = RectF(layer().left, textRegion().y - h * 0.5, layer().width, h);
		}
		else {
			m_fieldRect = RectF(layer().left, textRegion().y - 3_px, layer().width, textRegion().h + 6_px);
		}
	}

	String UIInputField::updateText() {
		String str = text();

		const auto raw = TextInput::GetRawInput();

		m_cursorPos = TextInput::UpdateText(str, m_cursorPos, TextInputMode::AllowBackSpaceDelete);

		// Fix forbidden characters
		const auto fixedStr = str.removed_if([this](const char32& c) {
			return forbiddenCharacters.includes(c);
			});

		if (fixedStr != str) {
			const auto diff = str.length() - fixedStr.length();
			m_cursorPos -= diff;
			fireForbiddenCharsNotifier();
		}

		return fixedStr;
	}

	void UIInputField::updateCursorMovement() {
		const bool leftMoveable = m_cursorPos > 0;
		const bool rightMoveable = m_cursorPos < text().length();

		const bool pressDurationElapsed = m_cursorMoveDurationWatcher.ms() > m_cursorMoveDuration;

		const bool leftDown = leftMoveable && KeyLeft.down();
		const bool leftPress = leftMoveable && pressDurationElapsed && KeyLeft.pressed();

		const bool rightDown = rightMoveable && KeyRight.down();
		const bool rightPress = rightMoveable && pressDurationElapsed && KeyRight.pressed();

		if (leftDown || rightDown) {
			m_cursorMoveDuration = 500;
			m_cursorMoveDurationWatcher.restart();
		}
		else if (leftPress || rightPress) {
			m_cursorMoveDuration = 30;
			m_cursorMoveDurationWatcher.restart();
		}

		if (leftDown || leftPress) {
			m_cursorPos--;
		}

		if (rightDown || rightPress) {
			m_cursorPos++;
		}

		if (leftDown || leftPress || rightDown || rightPress) {
			m_cursorMoveDurationWatcher.restart();
			m_isCursorVisible = true;
			m_cursorBeamWatcher.restart();
		}
	}

	void UIInputField::fireForbiddenCharsNotifier() {
		if (ui_Warning == nullptr) {
			ui_Warning = &AobaFactory::Create<UIText>();
			ui_Warning->backgroundColor = DynamicColor::DefaultYellow;
			ui_Warning->textColor = Palette::Black;
			ui_Warning->setDirection(TextDirection::Center);
			ui_Warning->setCornerRadius(5);
			ui_Warning->setText(U"この文字は許可されていません");
		}

		AobaCore::Instance().stopTimeout(m_WarningTimeoutID);

		ui_Warning->exist = true;
		ui_Warning->setConstraint(LayerDirection::Top, *this, LayerDirection::Bottom);
		ui_Warning->setConstraint(LayerDirection::Height, 30_px);
		ui_Warning->setConstraint(LayerDirection::CenterX, *this, LayerDirection::CenterX);
		ui_Warning->setConstraint(LayerDirection::Width, 250_px);
		m_WarningTimeoutID = AobaCore::Instance().setTimeout([this] {
			ui_Warning->exist = false;
			}, 3000, false);

		registerInputEvent(ForbiddenCharInputted(this, false));
	}

	void UIInputField::updateCursorBeamPos() {
		m_cursorBeamPosX = textRegion().x;

		for (const auto& [i, glyph] : Indexed(font().getGlyphs(text()))) {
			if (i == m_cursorPos) {
				break;
			}

			m_cursorBeamPosX += glyph.xAdvance;
		}
	}
}
