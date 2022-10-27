#pragma once

#include "UIButton.hpp"

namespace s3d::aoba {
    class UIToggleButton : public UIButton {
    public:
        static ColorTheme DefaultSelectedColor, DefaultSelectedTextColor;

        ColorTheme selectedColor = DefaultSelectedColor, selectedTextColor = DefaultSelectedTextColor;

        std::function<void(bool)> onChange = nullptr;

    private:
        bool m_enabled = false;

    public:
        using UIButton::UIButton;

        bool isEnabled() const noexcept {
            return m_enabled;
        }

        void setEnabled(bool enabled) noexcept {
            if (!m_enabled && enabled) {
                backgroundColor.highlight(selectedColor);
                textColor.highlight(selectedTextColor);
            } else if (m_enabled && !enabled) {
                backgroundColor.lowlight(defaultColor);
                textColor.highlight(defaultTextColor);
            }

            if (onChange && m_enabled != enabled) {
                m_enabled = enabled;
                onChange(m_enabled);
            }
        }

    protected:
        void initialize() override;
    };
}
