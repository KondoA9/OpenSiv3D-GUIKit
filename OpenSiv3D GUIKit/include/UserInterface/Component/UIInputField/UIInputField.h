#pragma once
#include "../UIText/UIText.h"
#include "../../Font/UnifiedFont.h"

namespace s3d::gui {
	class UIInputField : public UIText {
	private:
		bool m_focused = false; 
		double m_cursorVisibleTimer = 0.0;
		bool m_isCursorVisible = true;

	public:
		using UIText::UIText;

		void draw() override;

		bool mouseLeftDown() override;
	};
}