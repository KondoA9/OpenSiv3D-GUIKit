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
		explicit UIButton(
			const ColorTheme& _defaultColor = DefaultColor,
			const ColorTheme& _defaultTextColor = DefaultTextColor,
			const ColorTheme& _hoveredColor = DefaultHoveredColor,
			const ColorTheme& _hoveredTextColor = DefaultHoveredTextColor) noexcept :
			UIText(_defaultColor, _defaultTextColor),
			defaultColor(_defaultColor),
			hoveredColor(_hoveredColor),
			defaultTextColor(_defaultTextColor),
			hoveredTextColor(_hoveredTextColor)
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
