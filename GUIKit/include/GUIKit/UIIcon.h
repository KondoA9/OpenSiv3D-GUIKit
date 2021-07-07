#pragma once

#include "UIComponent.h"
#include "UIRect.h"

namespace s3d::gui {
	class UIIcon :public UIRect {
	public:
		ColorTheme iconColor;
		bool rotate = false;
		double rotationAnglerVelocity = 0.0;

	private:
		Texture m_icon;
		double m_angle = 0.0;

	public:
		UIIcon(Icon icon, const ColorTheme& iconColor = DynamicColor::Text, const ColorTheme& backgroundColor = DynamicColor::Clear):
			UIRect(backgroundColor),
			m_icon(icon),
			iconColor(iconColor)
		{}

		void setAngle(double angle) {
			m_angle = angle;
		}

		void resetAngle() {
			m_angle = 0.0;
		}

		void setIcon(const Icon& icon) {
			m_icon = Texture(icon);
		}

		void setIcon(const Texture& icon) {
			m_icon = icon;
		}

	protected:
		void draw() override;
	};
}
