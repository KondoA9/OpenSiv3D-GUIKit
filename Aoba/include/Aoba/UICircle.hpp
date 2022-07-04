#pragma once

#include "UIComponent.hpp"

namespace s3d::aoba {
    class UICircle : public UIComponent {
    private:
        Ellipse m_circle;

    public:
        using UIComponent::UIComponent;

        const Ellipse& circle() const noexcept {
            return m_circle;
        }

    protected:
        void updateMouseIntersection() final;

        void updateLayer(const Rect& scissor) override;

        void draw() const override;
    };
}
