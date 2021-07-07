#include <GUIKit/UICircle.h>

using namespace s3d::gui;

void UICircle::updateLayer(const Rect& scissor) {
	UIComponent::updateLayer(scissor);
	m_circle = Circle(
		Arg::center(static_cast<int>(m_layer.centerX), static_cast<int>(m_layer.centerY)),
		(m_layer.height + m_layer.width) * 0.25);
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
	m_mouseLeftDown = m_circle.leftClicked();
	m_mouseLeftUp = m_circle.leftReleased();
	m_mouseLeftPress = m_circle.leftPressed();

	m_mouseRightDown = m_circle.rightClicked();
	m_mouseRightUp = m_circle.rightReleased();
	m_mouseRightPress = m_circle.rightPressed();

	m_preMouseOver = m_mouseOver;
	m_mouseOver = m_circle.mouseOver();
}
