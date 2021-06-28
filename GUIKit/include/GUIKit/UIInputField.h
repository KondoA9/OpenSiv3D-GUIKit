#pragma once
#include "UIText.h"
#include "UnifiedFont.h"

namespace s3d::gui {
	GUICreateInputEvent(KeyDown);

	class UIInputField : public UIText {
	private:
		double m_cursorVisibleTimer = 0.0;
		bool m_isCursorVisible = true;

	public:
		using UIText::UIText;

	protected:
		void draw() override;

		void updateInputEvents() override;
	};
}