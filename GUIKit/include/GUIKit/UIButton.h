#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIButton : public UIRect {
	public:
		String title;
		Texture icon;
		ColorTheme textColor, defaultColor, defaultTextColor, hoveredColor;

	public:
		UIButton(const String& title, const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& defaultTextColor = Palette::White);

		UIButton(
			const String& title,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& defaultTextColor = Palette::White);

		UIButton(
			const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& defaultTextColor = Palette::White);

		UIButton(
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& defaultTextColor = Palette::White);

	protected:
		void draw() override;
	};
}