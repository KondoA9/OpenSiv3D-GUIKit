#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba {
    enum class ColorMode { Light, Dark };

    struct ColorTheme {
    public:
        Color mutable light, dark;

    private:
        Color m_transitionLight, m_transitionDark;
        double mutable m_transitionTimer = 0.0, m_transitionTime = 0.0;
        bool mutable m_isTransition = false;

    public:
        constexpr ColorTheme() = delete;

        constexpr ColorTheme(const Color& lightColor, const Color& darkColor) noexcept :
            light(lightColor), dark(darkColor), m_transitionLight(lightColor), m_transitionDark(darkColor) {}

        constexpr ColorTheme(Color&& lightColor, Color&& darkColor) noexcept :
            light(lightColor), dark(darkColor), m_transitionLight(lightColor), m_transitionDark(darkColor) {}

        constexpr ColorTheme(const Color& color) noexcept : ColorTheme(color, color) {}

        constexpr ColorTheme(Color&& color) noexcept : ColorTheme(color, color) {}

        constexpr ColorTheme(const ColorTheme&) noexcept = default;

        constexpr ColorTheme(ColorTheme&&) noexcept = default;

        ColorTheme& operator=(const ColorTheme&) noexcept = default;

        ColorTheme& operator=(ColorTheme&&) noexcept = default;

        Color color() const noexcept;

        void setColor(const Color& lightColor, const Color& darkColor, double transitionTime = 0.0) noexcept;

        void setColor(const Color& color, double transitionTime = 0.0) noexcept {
            setColor(color, color, transitionTime);
        }

        void setColor(const ColorTheme& color, double transitionTime = 0.0) noexcept {
            setColor(color.light, color.dark, transitionTime);
        }

        void highlight(const Color& lightColor, const Color& darkColor) noexcept {
            setColor(lightColor, darkColor, 0.25);
        }

        void highlight(const Color& color) noexcept {
            setColor(color, 0.25);
        }

        void highlight(const ColorTheme& color) noexcept {
            setColor(color, 0.25);
        }

        void lowlight(const Color& lightColor, const Color& darkColor) noexcept {
            setColor(lightColor, darkColor, 0.1);
        }

        void lowlight(const Color& color) noexcept {
            setColor(color, 0.1);
        }

        void lowlight(const ColorTheme& color) noexcept {
            setColor(color, 0.1);
        }

        operator Color() const noexcept {
            return color();
        }

        operator ColorF() const noexcept {
            return static_cast<ColorF>(color());
        }

        bool operator==(const Color& color) const noexcept {
            return this->color() == color;
        }

        static ColorMode CurrentColorMode() noexcept;

        static void SetColorMode(ColorMode mode) noexcept;

        static void Animate(double t) noexcept;
    };
}
