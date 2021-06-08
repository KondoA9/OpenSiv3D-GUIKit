#pragma once

#include "../UIComponent.h"
#include "../UIRect/UIRect.h"

namespace s3d::gui {
	class UIIcon :public UIRect {
	public:
		ColorTheme iconColor;
		bool rotate = false;
		double rotationAnglerVelocity = 0.0;

	private:
		Texture m_icon;
		double angle = 0.0;

	public:
		UIIcon(Icon icon, const ColorTheme& iconColor = DynamicColor::Text, const ColorTheme& backgroundColor = DynamicColor::Clear):
			UIRect(backgroundColor),
			m_icon(icon),
			iconColor(iconColor)
		{}

		void draw() override;
	};
}