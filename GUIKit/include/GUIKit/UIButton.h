#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIButton : public UIRect {
	public:
		ColorTheme defaultColor, hoveredColor, textColor;

	private:
		String m_title = U"";
		Texture m_icon;

	public:
		UIButton(
			const ColorTheme& _defaultColor = DynamicColor::Background,
			const ColorTheme& _textColor = DynamicColor::Text,
			const ColorTheme& _hoveredColor = DynamicColor::BackgroundSecondary) noexcept :
			UIRect(_defaultColor),
			defaultColor(_defaultColor),
			hoveredColor(_hoveredColor),
			textColor(_textColor)
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

		void setIcon(const Icon& icon) {
			m_icon = Texture(icon);
		}

		void setIcon(const Texture& icon) {
			m_icon = icon;
		}

	protected:
		void initialize() override;

		void draw() override;
	};
}
