#pragma once

#include "UIRect.h"
#include "Font/UnifiedFont.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIButton : public UIRect {
	public:
		String title;
		Texture icon;
		ColorTheme defaultColor, hoveredColor, textColor;

	public:
		UIButton(const String& title, const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::buttonDefault,
			const ColorTheme& hoveredColor = DynamicColor::buttonPushed,
			const ColorTheme& textColor = Palette::White) :
			UIRect(defaultColor),
			title(title),
			icon(icon),
			defaultColor(defaultColor),
			hoveredColor(hoveredColor),
			textColor(textColor)
		{}

		UIButton(
			const String& title, 
			const ColorTheme& defaultColor = DynamicColor::buttonDefault,
			const ColorTheme& hoveredColor = DynamicColor::buttonPushed,
			const ColorTheme& textColor = Palette::White) :
			UIButton(title, Texture(), defaultColor, hoveredColor, textColor)
		{}

		UIButton(
			const Texture& _icon, 
			const ColorTheme& _defaultColor = DynamicColor::buttonDefault,
			const ColorTheme& _hoveredColor = DynamicColor::buttonPushed,
			const ColorTheme& textColor = Palette::White) :
			UIButton(U"", _icon, _defaultColor, _hoveredColor, textColor)
		{}

		UIButton(
			const ColorTheme& defaultColor = DynamicColor::buttonDefault,
			const ColorTheme& hoveredColor = DynamicColor::buttonPushed,
			const ColorTheme& textColor = Palette::White) :
			UIButton(U"", defaultColor, hoveredColor, textColor)
		{}

		void draw() override {
			UIRect::draw();

			UnifiedFont::Get(UnifiedFontStyle::Medium)(title).drawAt(rect.center(), textColor);

			if (icon) {
				icon.drawAt(rect.center());
			}
		}

	private:
		void hovered() override {
			if (rect.mouseOver()) {
				Cursor::RequestStyle(CursorStyle::Hand);
				backgroundColor = hoveredColor;
			}
			else {
				backgroundColor = defaultColor;
			}
			if (onHovered) {
			}
		}
	};
}