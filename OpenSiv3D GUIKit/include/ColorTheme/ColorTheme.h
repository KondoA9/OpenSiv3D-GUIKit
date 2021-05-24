#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	enum class ColorMode {
		Light,
		Dark
	};

	struct ColorTheme {
		Color light, dark;

		constexpr ColorTheme(const Color& lightColor, const Color& darkColor) :
			light(lightColor),
			dark(darkColor)
		{}

		constexpr ColorTheme(const Color& color) :
			ColorTheme(color, color)
		{}

		Color color() const;

		operator Color() const {
			return color();
		}

		operator ColorF() const {
			return static_cast<ColorF>(color());
		}
		
		static void setColorMode(ColorMode mode);
		static ColorMode colorMode();
		static void animate(double _t);
	};
}