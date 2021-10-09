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

		addEventListener<UnFocused>([this] {
			if (!prefix.empty() && !text().starts_with(prefix)) {
				setText(prefix + text());
			}
			if (!suffix.empty() && !text().ends_with(suffix)) {
				setText(text() + suffix);
			}
			}, true);
	}

	void UIInputField::update() {
		UIText::update();

		if (isFocused()) {
			m_cursorVisibleTimer += Scene::DeltaTime();
			if (m_cursorVisibleTimer > 0.5) {
				m_cursorVisibleTimer = 0.0;
				m_isCursorVisible = !m_isCursorVisible;
			}
		}
	}

	void UIInputField::draw() const {
		m_fieldRect.draw(DynamicColor::BackgroundSecondary);

		UIText::draw();

		if (isFocused()) {
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
			const String previousText = text();

			const String updatedText = updateText(previousText, TextInput::GetRawInput(), getInputtedRawText());

			if (previousText != updatedText) {
				setText(updatedText);
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

	String UIInputField::getInputtedRawText() {
		String inputtedRawText = text();
		TextInput::UpdateText(inputtedRawText, TextInputMode::AllowBackSpaceDelete);
		return inputtedRawText;
	}

	String UIInputField::updateText(const String&, const String&, const String& rawUpdatedString) {
		const auto updatedString = rawUpdatedString.removed_if([this](const char32& c) {
			return forbiddenCharacters.includes(c);
			});

		if (updatedString != rawUpdatedString) {
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

		return updatedString;
	}
}
