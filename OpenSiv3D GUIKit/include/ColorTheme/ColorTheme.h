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
			if (m_isTransition) {
				const auto c = color();
				light = c;
				dark = c;
			}

			if (transitionTime <= 0.0) {
				light = lightColor;
				dark = darkColor;
			}

			if (light.a == 0) {
				light.setRGB(lightColor.r, lightColor.g, lightColor.b);
			}

			if (dark.a == 0) {
				dark.setRGB(darkColor.r, darkColor.g, darkColor.b);
			}

			m_transitionLight = lightColor.a == 0 ? Color(light, 0) : lightColor;
			m_transitionDark = darkColor.a == 0 ? Color(dark, 0) : darkColor;

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

		void highlight(const Color& lightColor, const Color& darkColor) {
			setColor(lightColor, darkColor, 0.25);
		}

		void highlight(const Color& color) {
			setColor(color, 0.25);
		}

		void highlight(const ColorTheme& color) {
			setColor(color,  0.25);
		}

		void lowlight(const Color& lightColor, const Color& darkColor) {
			setColor(lightColor, darkColor, 0.1);
		}

		void lowlight(const Color& color) {
			setColor(color, 0.1);
		}

		void lowlight(const ColorTheme& color) {
			setColor(color, 0.1);
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