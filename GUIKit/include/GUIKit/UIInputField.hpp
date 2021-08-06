#pragma once

#include "UIText.hpp"
#include "UnifiedFont.hpp"

namespace s3d::gui {
	class UIInputField : public UIText {
	public:
		GUICreateInputEvent(Inputted);
		GUICreateInputEvent(KeyEnterDown);

		bool completeInputWhenEnter = true;
		String prefix = U"", suffix = U"";

	private:
		Array<String> m_forbiddenWords;
		double m_cursorVisibleTimer = 0.0;
		bool m_isCursorVisible = true;
		RectF m_fieldRect;

	public:
		using UIText::UIText;

		const RectF& fieldRect() const {
			return m_fieldRect;
		}

		void setForbiddenWords(const Array<String>& words) {
			m_forbiddenWords = words;
		}

	protected:
		void initialize() override;

		void draw() override;

		void updateInputEvents() override;

		void updateDrawableText(bool updateField = false) override;
	};
}
