#include <GUIKit/UICircle.h>

using namespace s3d::gui;

void UICircle::updateLayer(const Rect& scissor) {
	UIComponent::updateLayer(scissor);
	m_circle = Circle(
		Arg::center(static_cast<int>(layer().centerX), static_cast<int>(layer().centerY)),
		(layer().height + layer().width) * 0.25);
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
