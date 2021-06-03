#pragma once

#include "../UIButton/UIButton.h"

namespace s3d::gui {
	class UIToggleButton : public UIButton {
	private:
		bool m_enabled = false;

	public:
		UIToggleButton(const String& title, const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White);

		UIToggleButton(
			const String& title,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White);

		UIToggleButton(
			const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White);

		UIToggleButton(
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White);

		bool isEnabled() const {
			return m_enabled;
		}

	protected:
		bool mouseHovered() override;

		bool mouseUnHovered() override;

		bool mouseHovering() override;

		bool mouseLeftDown() override;
	};
}