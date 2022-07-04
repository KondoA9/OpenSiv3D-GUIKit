#pragma once

#include "UIComponent.hpp"

namespace s3d::aoba {
    class UIRect : public UIComponent {
    private:
        RoundRect m_rect = RoundRect();

    public:
        using UIComponent::UIComponent;

        double cornerRadius() const noexcept {
            return m_rect.r;
        }

        void setCornerRadius(double r) noexcept;

    protected:
        void updateMouseIntersection() override;

        void updateLayer(const Rect& scissor) override;

        void draw() const override;
    };
}
