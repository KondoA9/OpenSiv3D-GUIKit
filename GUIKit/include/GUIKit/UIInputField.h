#pragma once

#include "UIText.h"
#include "UnifiedFont.h"

namespace s3d::gui {
	class UIInputField : public UIText {
	public:
		GUICreateInputEvent(KeyDown);

	private:
		double m_cursorVisibleTimer = 0.0;
		bool m_isCursorVisible = true;
		RectF m_fieldRect;

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
	};
}
