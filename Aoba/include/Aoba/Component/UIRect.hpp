#pragma once

#include "Aoba/Component/UIComponent.hpp"

namespace s3d::aoba {
    class UIRect : public UIComponent {
    private:
        RoundRect m_rect = RoundRect();

    public:
        using UIComponent::UIComponent;

        double cornerRadius() const {
            return m_rect.r;
        }

        void setCornerRadius(double r);

    protected:
        void updateMouseIntersection() override;

        void updateLayer(const Rect& scissor) override;

        void draw() const override;
    };
}
