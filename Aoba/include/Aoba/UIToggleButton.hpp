#pragma once

#include "UIButton.hpp"

namespace s3d::aoba {
	class UIToggleButton : public UIButton {
	public:
		static ColorTheme DefaultSelectedColor, DefaultSelectedTextColor;

		ColorTheme
			selectedColor = DefaultSelectedColor,
			selectedTextColor = DefaultSelectedTextColor;

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
