#pragma once

#include "UIText.hpp"

namespace s3d::aoba {
	class UIButton : public UIText {
	public:
		static ColorTheme DefaultColor, DefaultTextColor, DefaultHoveredColor, DefaultHoveredTextColor;

		ColorTheme
			defaultColor = DefaultColor,
			hoveredColor = DefaultHoveredColor,
			defaultTextColor = DefaultTextColor,
			hoveredTextColor = DefaultHoveredTextColor;

	private:
		Texture m_icon;

	public:
		using UIText::UIText;

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
