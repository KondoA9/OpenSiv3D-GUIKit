#pragma once

#include "Aoba/Component/UIBase.hpp"

namespace s3d::aoba {
    class UIRect : public UIBase {
    private:
        RoundRect m_rect = RoundRect();

    public:
        using UIBase::UIBase;

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
