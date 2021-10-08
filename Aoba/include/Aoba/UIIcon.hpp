#pragma once

#include "UIComponent.hpp"
#include "UIRect.hpp"

namespace s3d::aoba {
	class UIIcon :public UIRect {
	public:
		ColorTheme iconColor;
		bool rotate = false;
		double rotationAnglerVelocity = 0.0;

	private:
		Texture m_icon;
		double m_angle = 0.0;

	public:
		explicit UIIcon(const ColorTheme& _backgroundColor = DynamicColor::Clear, const ColorTheme& _iconColor = DynamicColor::Text) noexcept :
			UIRect(_backgroundColor),
			iconColor(_iconColor)
		{}

		void setAngle(double angle) {
			m_angle = angle;
		}

		void resetAngle() {
			m_angle = 0.0;
		}

		void setIcon(const Icon& icon, int32 size) {
			m_icon = Texture(icon, size);
		}

		void setIcon(const Texture& icon) {
			m_icon = icon;
		}

	protected:
		void update() override;

		void draw() const override;
	};
}
