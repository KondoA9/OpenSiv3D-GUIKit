#pragma once

#include "UIButton.h"

namespace s3d::gui {
	class UIToggleButton : public UIButton {
	public:
		ColorTheme selectedColor, selectedTextColor;

	private:
		bool m_enabled = false;

	public:
		UIToggleButton(const String& title, const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White,
			const ColorTheme& selectedColor = DynamicColor::ButtonPushed,
			const ColorTheme& selectedTextColor = Palette::White);

		UIToggleButton(
			const String& title,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White,
			const ColorTheme& selectedColor = DynamicColor::ButtonPushed,
			const ColorTheme& selectedTextColor = Palette::White);

		UIToggleButton(
			const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White,
			const ColorTheme& selectedColor = DynamicColor::ButtonPushed,
			const ColorTheme& selectedTextColor = Palette::White);

		UIToggleButton(
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White,
			const ColorTheme& selectedColor = DynamicColor::ButtonPushed,
			const ColorTheme& selectedTextColor = Palette::White);

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
	};
}