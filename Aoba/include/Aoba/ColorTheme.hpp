﻿#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba {
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
		bool mutable m_isTransition = false;

	public:
		constexpr ColorTheme() = delete;

		constexpr ColorTheme(const ColorTheme&) noexcept = default;

		constexpr ColorTheme(ColorTheme&&) noexcept = default;

		constexpr ColorTheme(const Color& lightColor, const Color& darkColor) noexcept :
			light(lightColor),
			dark(darkColor),
			m_transitionLight(lightColor),
			m_transitionDark(darkColor)
		{}

		constexpr ColorTheme(Color&& lightColor, Color&& darkColor) noexcept :
			light(lightColor),
			dark(darkColor),
			m_transitionLight(lightColor),
			m_transitionDark(darkColor)
		{}

		constexpr ColorTheme(const Color& color) noexcept :
			ColorTheme(color, color)
		{}

		constexpr ColorTheme(Color&& color) noexcept :
			ColorTheme(color, color)
		{}

		ColorTheme& operator =(const ColorTheme&) noexcept = default;

		ColorTheme& operator =(ColorTheme&&) noexcept = default;

		Color color() const;

		void setColor(const Color& lightColor, const Color& darkColor, double transitionTime = 0.0);

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
			setColor(color, 0.25);
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

		bool operator ==(const Color& color) const {
			return this->color() == color;
		}

		static ColorMode CurrentColorMode();

		static void SetColorMode(ColorMode mode);

		static void Animate(double t);
	};
}
