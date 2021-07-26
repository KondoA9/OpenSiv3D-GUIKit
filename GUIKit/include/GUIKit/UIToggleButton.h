#pragma once

#include "UIButton.h"

namespace s3d::gui {
	class UIToggleButton : public UIButton {
	public:
		ColorTheme selectedColor = DynamicColor::ButtonPushed, defaultTextColor = DynamicColor::Text, selectedTextColor = Palette::White;

	private:
		bool m_enabled = false;

	public:
		using UIButton::UIButton;

		bool isEnabled() const {
			return m_enabled;
		}

		void setEnabled(bool enabled) {
			if (!m_enabled && enabled) {
				backgroundColor.highlight(selectedColor);
				textColor.highlight(selectedTextColor);
			}
			else if (m_enabled && !enabled) {
				backgroundColor.lowlight(defaultColor);
				textColor.highlight(defaultTextColor);
			}
			m_enabled = enabled;
		}

	protected:
		void initialize() override;
	};
}
