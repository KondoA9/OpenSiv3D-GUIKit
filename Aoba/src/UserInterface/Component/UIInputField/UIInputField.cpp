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

			updateTextSelecting();

			updateCursorMovement();

			if (m_selectingByKeyboard) {
				m_textSelected = m_selectingCursorStart != m_cursorPos; 
			}

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

			if (m_textSelected) {
				drawSelectingArea();
			}
		}
	}

	void UIInputField::drawSelectingArea() const {
		const size_t start = Min(m_selectingCursorStart, m_cursorPos);
		const size_t end = Max(m_selectingCursorStart, m_cursorPos);

		double startPos = textRegion().x;
		double width = 0;

		for (const auto& [i, glyph] : Indexed(font().getGlyphs(text()))) {
			if (i < start) {
				startPos += glyph.xAdvance;
			}
			else {
				if (i < end) {
					width += glyph.xAdvance;
				}
				else {
					break;
				}
			}
		}

		Rect(startPos, textRegion().y, width, textRegion().h).draw({ 0, 0, 1, 0.5 });
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
		auto editingString = text();

		const auto raw = TextInput::GetRawInput();

		m_cursorPos = TextInput::UpdateText(editingString, m_cursorPos, TextInputMode::AllowBackSpaceDelete);

		// Fix forbidden characters
		{
			const auto fixedStr = editingString.removed_if([this](const char32& c) {
				return forbiddenCharacters.includes(c);
				});

			if (fixedStr != editingString) {
				const auto diff = editingString.length() - fixedStr.length();
				m_cursorPos -= diff;
				fireForbiddenCharsNotifier();
			}

			editingString = fixedStr;
		}

		// Limit max length
		if (type == Type::Number && !editingString.isEmpty()) {
			maxLength = (editingString.starts_with('-') | editingString.includes('.')) ? 9 : 8;
		}

		// Fix if reached max length
		if (editingString.length() > maxLength) {
			const auto diff = editingString.length() - maxLength;
			editingString.erase(m_cursorPos - diff, diff);
			m_cursorPos -= diff;
		}

		// Validate
		if (const auto result = validateStr(editingString); result.fixedText.has_value()) {
			const auto fixed = result.fixedText.value();
			const auto fixedLen = fixed.length();
			const auto editingLen = editingString.length();
			size_t diff = 0;
			for (size_t i : step(m_cursorPos)) {
				if (fixedLen > i && editingLen > i + diff) {
					if (editingString[i] != fixed[i]) {
						diff++;
					}
				}
			}
			editingString = fixed;
			m_cursorPos -= diff;
		}

		return editingString;
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
			if (!m_selectingByKeyboard) {
				// Reset selecting
				m_textSelected = false;
				m_selectingCursorStart = m_cursorPos;
			}
		}
	}

	void UIInputField::updateTextSelecting() {
		if (KeyShift.down()) {
			m_selectingByKeyboard = true;
			m_selectingCursorStart = m_cursorPos;
		}

		if (KeyShift.up()) {
			m_selectingByKeyboard = false;
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

	UIInputField::ValidateResult UIInputField::validateStr(const String& str) {
		switch (type)
		{
		case s3d::aoba::UIInputField::Type::Number:
			return validateNumber(str);

		default:
			return { none };
		}
	}

	UIInputField::ValidateResult UIInputField::validateNumber(const String& str) {
		// Remove chars if not '-', '.' or numbers
		String fixedText = str.removed_if([](const char32& c) {
			return !('0' <= c && c <= '9') && c != '-' && c != '.';
			});

		// Remove dots if integer
		if (numberType == NumberType::Integer) {
			fixedText.remove('.');
		}

		// Remove extra '-' and '.'
		{
			String tmp = U"";
			size_t dotCount = 0;

			for (const auto [i, c] : Indexed(fixedText)) {
				if (c == '.') {
					dotCount++;
				}

				const bool hyphenNG = c == '-' && i != 0;
				const bool dotNG = c == '.' && (i == 0 || dotCount > 1);

				if (!hyphenNG && !dotNG) {
					tmp += c;
				}
			}

			fixedText = tmp;
		}

		const bool minus = fixedText.starts_with('-');
		const bool withDot = fixedText.includes('.');

		if (fixedText.length() == 1 && minus) {
			return { fixedText };
		}

		// Clamp
		if (fixedText.length() > 0) {
			if (minNum >= 0 && minus) {
				fixedText.remove_at(0);
			}

			if (numberType == NumberType::Integer) {
				const int num = Clamp(ParseInt<int>(fixedText), static_cast<int>(minNum), static_cast<int>(maxNum));
				fixedText = Format(num);
			}
			else {
				if (!fixedText.ends_with('.')) {
					const double num = Clamp(ParseFloat<double>(fixedText), minNum, maxNum);
					if (num == 0.0 && withDot) {
						const auto len = fixedText.length() - 2;
						fixedText = U"0." + String(len > 5 ? 5 : len, '0');
					}
					else {
						fixedText = Format(num);
					}
				}
			}
		}

		return { fixedText };
	}
}
