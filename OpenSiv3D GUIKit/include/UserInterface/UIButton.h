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
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
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
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White) :
			UIButton(title, Texture(), defaultColor, hoveredColor, textColor)
		{}

		UIButton(
			const Texture& _icon, 
			const ColorTheme& _defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& _hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White) :
			UIButton(U"", _icon, _defaultColor, _hoveredColor, textColor)
		{}

		UIButton(
			const ColorTheme& defaultColor = DynamicColor::ButtonDefault,
			const ColorTheme& hoveredColor = DynamicColor::ButtonPushed,
			const ColorTheme& textColor = Palette::White) :
			UIButton(U"", defaultColor, hoveredColor, textColor)
		{}

		void draw() override {
			UIRect::draw();

			UnifiedFont::Get(UnifiedFontStyle::Medium)(title).drawAt(m_rect.center(), textColor);

			if (icon) {
				icon.drawAt(m_rect.center());
			}
		}

	private:
		bool hovered() override {
			if (UIRect::hovered()) {
				backgroundColor.highlight(hoveredColor);
				return true;
			}
			return false;
		}

		bool unHovered() override {
			if (UIRect::unHovered()) {
				backgroundColor.lowlight(defaultColor);
				return true;
			}
			return false;
		}

		bool hovering() override {
			if (UIRect::hovering()) {
				Cursor::RequestStyle(CursorStyle::Hand);
				return true;
			}
			return false;
		}
	};
}