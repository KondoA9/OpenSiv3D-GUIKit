#pragma once
#pragma once

#include "UIText.hpp"
#include "UnifiedFont.hpp"

namespace s3d::gui {
	class UIInputField : public UIText {
	public:
		GUICreateInputEvent(Inputted);
		GUICreateInputEvent(KeyEnterDown);
		GUICreateInputEvent(ForbiddenCharInputted);

		bool completeInputWhenEnter = true;
		String prefix = U"", suffix = U"";
		Array<char32> forbiddenCharacters = {};

		static const Array<char32> ForbiddenPathChar, ForbiddenPathCharRecommended;

	private:
		double m_cursorVisibleTimer = 0.0;
		bool m_isCursorVisible = true;
		RectF m_fieldRect;

		static UIText* ui_Warning;
		static size_t m_WarningTimeoutID;

	public:
		using UIText::UIText;

		const RectF& fieldRect() const {
			return m_fieldRect;
		}

	protected:
		void initialize() override;

		void draw() override;

		void updateInputEvents() override;

		void updateDrawableText(bool updateField = false) override;

		virtual String updateText(const String& previousString, const String& rawInput, const String& rawUpdatedString);

		String getInputtedRawText();
	};
}
