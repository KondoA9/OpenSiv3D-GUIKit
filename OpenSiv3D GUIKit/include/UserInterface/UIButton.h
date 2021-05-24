#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIButton : public UIRect {
	public:
		String m_title;
		Texture m_icon;
		ColorTheme m_defaultColor, m_hoveredColor, m_textColor;

	private:
		const SDFFont m_font;

	public:
		UIButton(const String& title, const Texture& icon,
			const ColorTheme& defaultColor = DynamicColor::buttonDefault,
			const ColorTheme& hoveredColor = DynamicColor::buttonPushed,
			const ColorTheme& textColor = Palette::White) :
			UIRect(defaultColor),
			m_title(title),
			m_icon(icon),
			m_defaultColor(defaultColor),
			m_hoveredColor(hoveredColor),
			m_textColor(textColor),
			m_font(SDFFont(60, Typeface::Light))
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

			const auto text = m_font(m_title);
			Graphics2D::SetSDFParameters(text.font.pixelRange());

			double fontsize = rect.h * 0.5;
			if (const auto region = text.regionAt(fontsize); region.w > rect.w * 0.5) {
				fontsize *= rect.w * 0.5 / region.w;
			}
			text.drawAt(fontsize, rect.center(), m_textColor);

			if (m_icon) {
				m_icon.drawAt(rect.center());
			}
		}

	private:
		void hovered() override {
			if (rect.mouseOver()) {
				Cursor::RequestStyle(CursorStyle::Hand);
				backgroundColor = m_hoveredColor;
			}
			else {
				backgroundColor = m_defaultColor;
			}
			if (onHovered) {
			}
		}
	};
}