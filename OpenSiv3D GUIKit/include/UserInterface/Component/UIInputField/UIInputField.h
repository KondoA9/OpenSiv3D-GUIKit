#pragma once
#include "../UIText/UIText.h"
#include "../../Font/UnifiedFont.h"

namespace s3d::gui {
	class UIInputField : public UIText {
	private:
		double m_cursorVisibleTimer = 0.0;
		bool m_isCursorVisible = true;
		std::function<void(const String&)> m_onInputtedHandler;

	public:
		using UIText::UIText;

		void setOnInputtedHandler(const std::function<void(const String&)>& func) {
			m_onInputtedHandler = func;
		}

	protected:
		void draw() override;
	};
}