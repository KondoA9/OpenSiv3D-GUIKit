#pragma once

#include <Siv3D.hpp>

#include "Constraint.hpp"

namespace s3d::aoba {
    enum class LayerDirection { Top, Bottom, CenterY, Height, Left, Right, CenterX, Width };

    struct Layer {
    private:
        Constraint m_top, m_bottom, m_centerY, m_height, m_left, m_right, m_centerX, m_width;

    public:
        Rect asRect() const noexcept {
            return asRectF().asRect();
        }

        RectF asRectF() const noexcept {
            return RectF(m_left, m_top, m_width, m_height);
        }

        Vec2 center() const noexcept {
            return Vec2(m_centerX, m_centerY);
        }

        const Constraint& top() const noexcept {
            return m_top;
        }

        const Constraint& bottom() const noexcept {
            return m_bottom;
        }

        const Constraint& centerY() const noexcept {
            return m_centerY;
        }

        const Constraint& height() const noexcept {
            return m_height;
        }

        const Constraint& left() const noexcept {
            return m_left;
        }

        const Constraint& right() const noexcept {
            return m_right;
        }

        const Constraint& centerX() const noexcept {
            return m_centerX;
        }

        const Constraint& width() const noexcept {
            return m_width;
        }

        void updateConstraints();

        void setConstraint(LayerDirection direction, double constant, double multiplier) noexcept;

        void setConstraint(LayerDirection direction,
                           const std::function<double()>& func,
                           double constant,
                           double multiplier);

        void setConstraint(LayerDirection direction,
                           Layer& otherLayer,
                           LayerDirection otherLayerDirection,
                           double constant,
                           double multiplier) noexcept;

        void removeConstraint(LayerDirection direction) noexcept;

        void removeAllConstraints() noexcept;

    private:
        Constraint* getConstraint(LayerDirection direction) noexcept;
    };
}
