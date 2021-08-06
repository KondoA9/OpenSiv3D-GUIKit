#include <GUIKit/UICircle.hpp>

namespace s3d::gui {
	void UICircle::updateLayer(const Rect& scissor) {
		UIComponent::updateLayer(scissor);
		m_circle = Ellipse(layer().centerX, layer().centerY, layer().width * 0.5, layer().height * 0.5);
	}

	void UICircle::draw() {
		if (fillInner) {
			m_circle.draw(backgroundColor);
		}
		if (drawFrame) {
			m_circle.drawFrame(0.0, frameThickness, frameColor);
		}
	}

	void UICircle::updateMouseIntersection() {
		_updateMouseCondition(
			m_circle.leftClicked(), m_circle.leftReleased(), m_circle.leftPressed(),
			m_circle.rightClicked(), m_circle.rightReleased(), m_circle.rightPressed(),
			m_circle.mouseOver()
		);
	}
}
