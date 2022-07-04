#include "Aoba/ColorTheme.hpp"

namespace s3d::aoba {
    ColorMode CurrentMode;
    double T = 0.0;

    Color ColorTheme::color() const noexcept {
        if (m_isTransition) {
            m_transitionTimer += Scene::DeltaTime();
            const double k = m_transitionTimer / m_transitionTime;
            if (k > 1.0) {
                light          = m_transitionLight;
                dark           = m_transitionDark;
                m_isTransition = false;
                return CurrentMode == ColorMode::Light ? light : dark;
            }

            return (CurrentMode == ColorMode::Light ? light : dark)
                .lerp(CurrentMode == ColorMode::Light ? m_transitionLight : m_transitionDark, k);
        } else {
            return light.lerp(dark, T);
        }
    }

    void ColorTheme::setColor(const Color& lightColor, const Color& darkColor, double transitionTime) noexcept {
        if (m_isTransition) {
            const auto c = color();
            light        = c;
            dark         = c;
        }

        if (transitionTime <= 0.0) {
            light = lightColor;
            dark  = darkColor;
        }

        if (light.a == 0) {
            light.setRGB(lightColor.r, lightColor.g, lightColor.b);
        }

        if (dark.a == 0) {
            dark.setRGB(darkColor.r, darkColor.g, darkColor.b);
        }

        m_transitionLight = lightColor.a == 0 ? Color(light, 0) : lightColor;
        m_transitionDark  = darkColor.a == 0 ? Color(dark, 0) : darkColor;

        m_transitionTime  = transitionTime;
        m_transitionTimer = 0.0;
        m_isTransition    = true;
    }

    ColorMode ColorTheme::CurrentColorMode() noexcept {
        return CurrentMode;
    }

    void ColorTheme::SetColorMode(ColorMode mode) noexcept {
        CurrentMode = mode;
    }

    void ColorTheme::Animate(double t) noexcept {
        T = t;
    }
}
