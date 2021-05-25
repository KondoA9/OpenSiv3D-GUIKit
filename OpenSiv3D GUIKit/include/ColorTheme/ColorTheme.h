#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	enum class ColorMode {
		Light,
		Dark
	};

	struct ColorTheme {
	public:
		Color mutable light, dark;

	private:
		Color m_transitionLight, m_transitionDark;
		double mutable m_transitionTimer = 0.0, m_transitionTime = 0.0;
		bool  mutable m_isTransition = false;

	public:
		constexpr ColorTheme(const Color& lightColor, const Color& darkColor) :
			light(lightColor),
			dark(darkColor),
			m_transitionLight(lightColor),
			m_transitionDark(darkColor)
		{}

		constexpr ColorTheme(const Color& color) :
			ColorTheme(color, color)
		{}

		Color color() const;

		void setColor(const Color& lightColor, const Color& darkColor, double transitionTime = 0.0) {
			if (transitionTime <= 0.0) {
				light = lightColor;
				dark = darkColor;
				return;
			}
			m_transitionLight = lightColor;
			m_transitionDark = darkColor;
			m_transitionTime = transitionTime;
			m_transitionTimer = 0.0;
			m_isTransition = true;
		}

		void setColor(const Color& color, double transitionTime = 0.0) {
			setColor(color, color, transitionTime);
		}

		void setColor(const ColorTheme& color, double transitionTime = 0.0) {
			setColor(color.light, color.dark, transitionTime);
		}

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