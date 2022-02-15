#include "Aoba/UIRect.hpp"

namespace s3d::aoba {
    void UIRect::setCornerRadius(double r) {
        m_rect = RoundRect(static_cast<int>(layer().left()),
                           static_cast<int>(layer().top()),
                           static_cast<int>(layer().width()),
                           static_cast<int>(layer().height()),
                           static_cast<int>(r));
    }

    void UIRect::draw() const {
        if (fillInner) {
            m_rect.draw(backgroundColor);
        }
        if (drawFrame) {
            m_rect.drawFrame(0.0, frameThickness, frameColor);
        }
    }

    void UIRect::updateLayer(const Rect& scissor) {
        UIComponent::updateLayer(scissor);

        m_rect = RoundRect(static_cast<int>(layer().left()),
                           static_cast<int>(layer().top()),
                           static_cast<int>(layer().width()),
                           static_cast<int>(layer().height()),
                           static_cast<int>(m_rect.r));
    }

    void UIRect::updateMouseIntersection() {
        _updateMouseCondition(m_rect.leftClicked(),
                              m_rect.leftReleased(),
                              m_rect.leftPressed(),
                              m_rect.rightClicked(),
                              m_rect.rightReleased(),
                              m_rect.rightPressed(),
                              m_rect.mouseOver());
    }
}
