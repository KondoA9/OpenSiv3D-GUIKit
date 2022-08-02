#include "Aoba/ColorTheme.hpp"

namespace s3d::aoba {
    ColorMode currentMode = ColorMode::Light;
    bool animating        = false;
    double lerpFactor     = 0.0;

    Color ColorTheme::color() const noexcept {
        if (m_isTransition) {
            m_transitionTimer += static_cast<uint64>(Scene::DeltaTime() * 1000);
            const double k = m_transitionTime == 0
                                 ? 1.0
                                 : static_cast<double>(m_transitionTimer) / static_cast<double>(m_transitionTime);
            if (k >= 1.0) {
                light          = m_transitionLight;
                dark           = m_transitionDark;
                m_isTransition = false;
                return currentMode == ColorMode::Light ? light : dark;
            }

            switch (currentMode) {
            case s3d::aoba::ColorMode::Light:
                return (animating ? dark : light).lerp(m_transitionLight, k);

            case s3d::aoba::ColorMode::Dark:
                return (animating ? light : dark).lerp(m_transitionDark, k);
            }

        } else if (animating) {
            switch (currentMode) {
            case s3d::aoba::ColorMode::Light:
                return dark.lerp(light, lerpFactor);

            case s3d::aoba::ColorMode::Dark:
                return light.lerp(dark, lerpFactor);
            }
        } else {
            return currentMode == ColorMode::Light ? light : dark;
        }

        assert(false);

        return currentMode == ColorMode::Light ? light : dark;
    }

    void ColorTheme::setColor(const Color& lightColor, const Color& darkColor, uint64 transitionTimeMs) noexcept {
        if (m_isTransition) {
            const auto c = color();
            light        = c;
            dark         = c;
        }

        if (transitionTimeMs == 0) {
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

        m_transitionTime  = transitionTimeMs;
        m_transitionTimer = 0;
        m_isTransition    = true;
    }

    ColorMode ColorTheme::CurrentMode() noexcept {
        return currentMode;
    }

    void ColorTheme::SetMode(ColorMode mode) noexcept {
        if (currentMode != mode) {
            currentMode = mode;
            animating   = true;
            lerpFactor  = 0.0;
        }
    }

    void ColorTheme::ToggleMode() noexcept {
        SetMode(currentMode == ColorMode::Light ? ColorMode::Dark : ColorMode::Light);
    }

    void ColorTheme::Update() noexcept {
        if (animating) {
            lerpFactor += 5.0 * Scene::DeltaTime();

            if (lerpFactor > 1.0) {
                lerpFactor = 1.0;
                animating  = false;
            }
        }
    }
}
