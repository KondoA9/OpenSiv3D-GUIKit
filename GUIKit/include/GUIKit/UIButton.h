#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIButton : public UIRect {
	public:
		String title = U"";
		Texture icon;
		ColorTheme defaultColor, defaultTextColor, hoveredColor, textColor = Palette::White;

	public:
		UIButton(
			const ColorTheme& _defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& _defaultTextColor = Palette::White,
			const ColorTheme& _hoveredColor = DynamicColor::ButtonPushed) noexcept :
			UIRect(_defaultColor),
			defaultColor(_defaultColor),
			defaultTextColor(_defaultTextColor),
			hoveredColor(_hoveredColor),
			textColor(_defaultTextColor)
		{}

		void setTextColor(const ColorTheme& color) {
			textColor = color;
		}

		void setTitle(const String& _title) {
			title = _title;
		}

		void setIcon(const Texture& _icon) {
			icon = _icon;
		}

	protected:
		void initialize() override;

		void draw() override;
	};
}
