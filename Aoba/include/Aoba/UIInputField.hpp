#pragma once

#include "UIText.hpp"
#include "UnifiedFont.hpp"

namespace s3d::aoba {
	class UIInputField : public UIText {
	public:
		AobaCreateInputEvent(Inputted);
		AobaCreateInputEvent(KeyEnterDown);
		AobaCreateInputEvent(ForbiddenCharInputted);

		String prefix = U"", suffix = U"";
		Array<char32> forbiddenCharacters = {};

		static const Array<char32> ForbiddenPathChar, ForbiddenPathCharRecommended;

	private:
		bool m_isCursorVisible = true;
		RectF m_fieldRect;

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

	protected:
		void initialize() override;

		void update() override;

		void updateInputEvents() override;

		void updateDrawableText(bool updateField = false) override;

		void draw() const override;

		virtual String updateText();

	private:
		void updateCursorMovement();

		void fireForbiddenCharsNotifier();

		void updateCursorBeamPos();
	};
}
