#pragma once

#include "UIText.hpp"

#include <Siv3D.hpp>

namespace s3d::aoba {
	class UIButton : public UIText {
	public:
		ColorTheme defaultColor, hoveredColor, defaultTextColor, hoveredTextColor;

	private:
		Texture m_icon;

	public:
		explicit UIButton(
			const ColorTheme& _defaultColor = DynamicColor::Background,
			const ColorTheme& _defaultTextColor = DynamicColor::Text,
			const ColorTheme& _hoveredColor = DynamicColor::BackgroundSecondary,
			const ColorTheme& _hoveredTextColor = DynamicColor::Text) noexcept :
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

		void draw() override;
	};
}
