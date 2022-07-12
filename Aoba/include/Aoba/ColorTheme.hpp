#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba {
    enum class ColorMode { Light, Dark };

    class ColorTheme {
        friend class Core;

    public:
        Color mutable light, dark;

    private:
        Color m_transitionLight, m_transitionDark;
        uint64 mutable m_transitionTimer = 0, m_transitionTime = 0;
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

        void setColor(const Color& lightColor, const Color& darkColor, uint64 transitionTimeMs = 0) noexcept;

        void setColor(const Color& color, uint64 transitionTimeMs = 0) noexcept {
            setColor(color, color, transitionTimeMs);
        }

        void setColor(const ColorTheme& color, uint64 transitionTimeMs = 0) noexcept {
            setColor(color.light, color.dark, transitionTimeMs);
        }

        void highlight(const Color& lightColor, const Color& darkColor) noexcept {
            setColor(lightColor, darkColor, 250);
        }

        void highlight(const Color& color) noexcept {
            setColor(color, 250);
        }

        void highlight(const ColorTheme& color) noexcept {
            setColor(color, 250);
        }

        void lowlight(const Color& lightColor, const Color& darkColor) noexcept {
            setColor(lightColor, darkColor, 100);
        }

        void lowlight(const Color& color) noexcept {
            setColor(color, 100);
        }

        void lowlight(const ColorTheme& color) noexcept {
            setColor(color, 100);
        }

        operator Color() const noexcept {
            return color();
        }

        ColorTheme& operator=(const Color& color) noexcept {
            light = dark = m_transitionLight = m_transitionDark = color;
            return *this;
        }

        ColorTheme& operator=(Color&& color) noexcept {
            light = dark = m_transitionLight = m_transitionDark = color;
            return *this;
        }

        operator ColorF() const noexcept {
            return static_cast<ColorF>(color());
        }

        bool operator==(const Color& color) const noexcept {
            return this->color() == color;
        }

        static ColorMode CurrentColorMode() noexcept;

    private:
        static void SetColorMode(ColorMode mode) noexcept;

        // t: light:0.0 ~ dark:1.0
        static void Animate(double t) noexcept;
    };
}
