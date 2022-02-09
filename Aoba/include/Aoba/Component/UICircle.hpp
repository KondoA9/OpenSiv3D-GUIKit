#pragma once

#include "UIBase.hpp"

namespace s3d::aoba {
    class UICircle : public UIBase {
    private:
        Ellipse m_circle;

    public:
        using UIBase::UIBase;

        const Ellipse& circle() const {
            return m_circle;
        }

    protected:
        void updateMouseIntersection() final;

        void updateLayer(const Rect& scissor) override;

        void draw() const override;
    };
}
