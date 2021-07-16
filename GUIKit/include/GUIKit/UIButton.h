#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIButton : public UIRect {
	public:
		ColorTheme defaultColor, defaultTextColor, hoveredColor, textColor = Palette::White;

	private:
		String m_title = U"";
		Texture m_icon;

	public:
		UIButton(
			const ColorTheme& _defaultColor = DynamicColor::Background,
			const ColorTheme& _defaultTextColor = DynamicColor::Text,
			const ColorTheme& _hoveredColor = DynamicColor::BackgroundSecondary) noexcept :
			UIRect(_defaultColor),
			defaultColor(_defaultColor),
			defaultTextColor(_defaultTextColor),
			hoveredColor(_hoveredColor),
			textColor(_defaultTextColor)
		{}

		const String& title() const {
			return m_title;
		}

		const Texture& icon() const {
			return m_icon;
		}

		void setTextColor(const ColorTheme& color) {
			textColor = color;
		}

		void setTitle(const String& _title) {
			m_title = _title;
		}

		void setIcon(const Texture& _icon) {
			m_icon = _icon;
		}

	protected:
		void initialize() override;

		void draw() override;
	};
}
