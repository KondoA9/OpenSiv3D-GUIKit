#include "Aoba/Layer.hpp"

#include "src/AobaLog/AobaLog.hpp"

namespace s3d::aoba {
    void Layer::updateConstraints() {
        // y-axis constraints
        // top & bottom
        if (m_top.isExist() && m_bottom.isExist()) {
            m_top.updateConstraint();
            m_bottom.updateConstraint();
            m_height  = m_bottom - m_top;
            m_centerY = m_top + m_height * 0.5;
        }
        // top / bottom & height
        else if (m_top.isExist() && m_height.isExist()) {
            m_top.updateConstraint();
            m_height.updateConstraint();
            m_bottom  = m_top + m_height;
            m_centerY = m_top + m_height * 0.5;
        } else if (m_bottom.isExist() && m_height.isExist()) {
            m_bottom.updateConstraint();
            m_height.updateConstraint();
            m_top     = m_bottom - m_height;
            m_centerY = m_top + m_height * 0.5;
        }
        // top / bottom / height & centerY
        else if (m_top.isExist() && m_centerY.isExist()) {
            m_top.updateConstraint();
            m_centerY.updateConstraint();
            m_height = (m_centerY - m_top) * 2;
            m_bottom = m_top + m_height;
        } else if (m_bottom.isExist() && m_centerY.isExist()) {
            m_bottom.updateConstraint();
            m_centerY.updateConstraint();
            m_height = (m_bottom - m_centerY) * 2;
            m_top    = m_bottom - m_height;
        } else if (m_height.isExist() && m_centerY.isExist()) {
            m_height.updateConstraint();
            m_centerY.updateConstraint();
            m_top    = m_centerY - m_height * 0.5;
            m_bottom = m_top + m_height;
        } else {
            AobaLog::Log(AobaLog::Type::Warning, U"Layer", U"Y-axis constraints are invalid");
        }

        // x-axis constraints
        // left & right
        if (m_left.isExist() && m_right.isExist()) {
            m_left.updateConstraint();
            m_right.updateConstraint();
            m_width   = m_right - m_left;
            m_centerX = m_left + m_width * 0.5;
        }
        // left / right & width
        else if (m_left.isExist() && m_width.isExist()) {
            m_left.updateConstraint();
            m_width.updateConstraint();
            m_right   = m_left + m_width;
            m_centerX = m_left + m_width * 0.5;
        } else if (m_right.isExist() && m_width.isExist()) {
            m_right.updateConstraint();
            m_width.updateConstraint();
            m_left    = m_right - m_width;
            m_centerX = m_left + m_width * 0.5;
        }
        // left / right / width & centerX
        else if (m_left.isExist() && m_centerX.isExist()) {
            m_left.updateConstraint();
            m_centerX.updateConstraint();
            m_width = (m_centerX - m_left) * 2;
            m_right = m_left + m_width;
        } else if (m_right.isExist() && m_centerX.isExist()) {
            m_right.updateConstraint();
            m_centerX.updateConstraint();
            m_width = (m_right - m_centerX) * 2;
            m_left  = m_right - m_width;
        } else if (m_width.isExist() && m_centerX.isExist()) {
            m_width.updateConstraint();
            m_centerX.updateConstraint();
            m_left  = m_centerX - m_width * 0.5;
            m_right = m_left + m_width;
        } else {
            AobaLog::Log(AobaLog::Type::Warning, U"Layer", U"X-axis constraints are invalid");
        }
    }

    void Layer::setConstraint(LayerDirection direction, double constant, double multiplier) {
        constraintReferenceTo(direction).setConstraint(constant, multiplier);
    }

    void Layer::setConstraint(LayerDirection direction,
                              const std::function<double()>& func,
                              double constant,
                              double multiplier) {
        constraintReferenceTo(direction).setConstraint(func, constant, multiplier);
    }

    void Layer::setConstraint(LayerDirection direction,
                              Layer& otherLayer,
                              LayerDirection otherLayerDirection,
                              double constant,
                              double multiplier) {
        double* const watchingValue = otherLayer.constraintReferenceTo(otherLayerDirection).data();
        constraintReferenceTo(direction).setConstraint(watchingValue, constant, multiplier);
    }

#pragma warning(disable : 4715)
    Constraint& Layer::constraintReferenceTo(LayerDirection direction) {
        switch (direction) {
        case s3d::aoba::LayerDirection::Top:
            return m_top;

        case s3d::aoba::LayerDirection::Bottom:
            return m_bottom;

        case s3d::aoba::LayerDirection::CenterY:
            return m_centerY;

        case s3d::aoba::LayerDirection::Height:
            return m_height;

        case s3d::aoba::LayerDirection::Left:
            return m_left;

        case s3d::aoba::LayerDirection::Right:
            return m_right;

        case s3d::aoba::LayerDirection::CenterX:
            return m_centerX;

        case s3d::aoba::LayerDirection::Width:
            return m_width;

        default:
            assert(false);
            break;
        }
    }
#pragma warning(default : 4715)

    void Layer::removeConstraint(LayerDirection direction) {
        constraintReferenceTo(direction).removeConstraint();
    }

    void Layer::removeAllConstraints() {
        m_top.removeConstraint();
        m_bottom.removeConstraint();
        m_centerY.removeConstraint();
        m_height.removeConstraint();
        m_left.removeConstraint();
        m_right.removeConstraint();
        m_centerX.removeConstraint();
        m_width.removeConstraint();
    }
}
