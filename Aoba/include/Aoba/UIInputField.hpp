#pragma once

#include "UIText.hpp"
#include "UnifiedFont.hpp"

namespace s3d::aoba {
	class UIInputField : public UIText {
	public:
		enum class Type {
			Text,
			Number
		};

		enum class NumberType {
			Integer,
			Decimals
		};

		struct ValidateResult {
			Optional<String> fixedText = none;
		};

		AobaCreateInputEvent(Inputted);
		AobaCreateInputEvent(KeyEnterDown);
		AobaCreateInputEvent(ForbiddenCharInputted);

		Type type = Type::Text;
		NumberType numberType = NumberType::Decimals;
		size_t maxLength = String::npos;
		double minNum = -DBL_MAX, maxNum = DBL_MAX;
		String prefix = U"", suffix = U"";
		Array<char32> forbiddenCharacters = {};

		static const Array<char32> ForbiddenPathChar, ForbiddenPathCharRecommended;

	private:
		bool m_textSelected = false;
		bool m_selectingByKeyboard = false;
		size_t m_selectingCursorStart = 0;

		bool m_isCursorVisible = true;
		RectF m_fieldRect, m_selectingRect;

		Stopwatch m_cursorBeamWatcher, m_cursorMoveDurationWatcher;
		int32 m_cursorMoveDuration = 500;
		size_t m_cursorPos = 0;
		double m_cursorBeamPosX = 0;

		static UIText* ui_Warning;
		static size_t m_WarningTimeoutID;

	public:
		using UIText::UIText;

		const RectF& fieldRect() const {
			return m_fieldRect;
		}

		double number() {
			if (text().length() == 1 && text().starts_with('-')) {
				return 0;
			}

			assert(type == Type::Number);
			return ParseFloat<double>(text());
		}

	protected:
		void initialize() override;

		void update() override;

		void updateInputEvents() override;

		void updateDrawableText(bool updateField = false) override;

		void draw() const override;

		virtual String updateText();

		virtual ValidateResult validateStr(const String& str);

	private:
		void updateCursorMovement();

		void updateTextSelecting();

		void fireForbiddenCharsNotifier();

		void updateCursorBeamPos();

		void updateSelectingRect();

		void updateTextControls();

		ValidateResult validateNumber(const String& str);
	};
}
