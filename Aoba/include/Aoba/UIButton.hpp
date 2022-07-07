#pragma once

#include "UIText.hpp"

namespace s3d::aoba {
    class UIButton : public UIText {
    public:
        static ColorTheme DefaultColor, DefaultTextColor, DefaultHoveredColor, DefaultHoveredTextColor;

        ColorTheme defaultColor = DefaultColor, hoveredColor = DefaultHoveredColor, defaultTextColor = DefaultTextColor,
                   hoveredTextColor = DefaultHoveredTextColor;

    private:
        Texture m_icon;

    public:
        using UIText::UIText;

        const Texture& icon() const noexcept {
            return m_icon;
        }

        void setIcon(const Texture& icon) noexcept {
            m_icon = icon;
        }

        void setIcon(const Icon& icon, int32 size) {
            setIcon(Texture(icon, size));
        }

    protected:
        void initialize() override;

        void initializeColors() override;

        void draw() const override;
    };
}
