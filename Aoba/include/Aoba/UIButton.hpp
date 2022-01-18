#pragma once

#include "UIText.hpp"

namespace s3d::aoba {
	class UIButton : public UIText {
	public:
		static ColorTheme DefaultColor, DefaultTextColor, DefaultHoveredColor, DefaultHoveredTextColor;

		ColorTheme defaultColor, hoveredColor, defaultTextColor, hoveredTextColor;

	private:
		Texture m_icon;

	public:
		UIButton() noexcept :
			UIText(),
			defaultColor(DefaultColor),
			defaultTextColor(DefaultTextColor),
			hoveredColor(DefaultHoveredColor),
			hoveredTextColor(DefaultHoveredTextColor)
		{
			setDirection(TextDirection::Center);
		}

		const Texture& icon() const {
			return m_icon;
		}

		void setIcon(const Icon& icon, int32 size) {
			m_icon = Texture(icon, size);
		}

		void setIcon(const Texture& icon) {
			m_icon = icon;
		}

	protected:
		void initialize() override;

		void draw() const override;
	};
}
