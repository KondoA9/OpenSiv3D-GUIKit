#pragma once

#include "UIButton.hpp"

namespace s3d::aoba {
	class UIToggleButton : public UIButton {
	public:
		ColorTheme selectedColor, selectedTextColor;

	private:
		bool m_enabled = false;

	public:
		explicit UIToggleButton(const ColorTheme& _selectedColor = DynamicColor::ButtonPushed, const ColorTheme& _selectedTextColor = Palette::White) :
			UIButton(DynamicColor::Background, DynamicColor::Text, DynamicColor::BackgroundSecondary, Palette::White),
			selectedColor(_selectedColor),
			selectedTextColor(_selectedTextColor)
		{}


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
