#pragma once

#include "../UIRect/UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIButton : public UIRect {
	public:
		String title;
		Texture icon;
		ColorTheme defaultColor, hoveredColor, textColor;

	public:
		UIButton(const String& title, const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White);

		UIButton(
			const String& title,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White);

		UIButton(
			const Texture& _icon,
			const ColorTheme& _defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& _hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White);

		UIButton(
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White);

	protected:
		void draw() override;

		bool mouseHovered() override;

		bool mouseUnHovered() override;

		bool mouseHovering() override;
	};
}