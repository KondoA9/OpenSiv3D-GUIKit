#pragma once

#include <Siv3D.hpp>

#include "Constraint.hpp"

namespace s3d::aoba {
    enum class LayerDirection { Top, Bottom, CenterY, Height, Left, Right, CenterX, Width };

    struct Layer {
    private:
        Constraint m_top, m_bottom, m_centerY, m_height, m_left, m_right, m_centerX, m_width;

        Vec2 m_center = Vec2();

    public:
        Rect asRect() const;

		RectF asRectF() const;

        const Vec2& center() const;

        const Constraint& top() const;

        const Constraint& bottom() const;

        const Constraint& centerY() const;

        const Constraint& height() const;

        const Constraint& left() const;

        const Constraint& right() const;

        const Constraint& centerX() const;

        const Constraint& width() const;

        void updateConstraints();

        void setConstraint(LayerDirection direction, double constant, double multiplier);

        void setConstraint(LayerDirection direction,
                           const std::function<double()>& func,
                           double constant,
                           double multiplier);

        void setConstraint(LayerDirection direction,
                           Layer& otherLayer,
                           LayerDirection otherLayerDirection,
                           double constant,
                           double multiplier);

        void removeConstraint(LayerDirection direction);

        void removeAllConstraints();

    private:
        Constraint& constraintReferenceTo(LayerDirection direction);
    };
}
