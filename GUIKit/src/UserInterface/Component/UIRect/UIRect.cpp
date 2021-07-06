#include <GUIKit/UIRect.h>

using namespace s3d::gui;

void UIRect::setCornerRadius(double r) {
	m_rect = RoundRect(
		static_cast<int>(m_layer.left),
		static_cast<int>(m_layer.top),
		static_cast<int>(m_layer.width),
		static_cast<int>(m_layer.height),
		static_cast<int>(r)
	);
}

void UIRect::draw() {
	if (fillInner) {
		m_rect.draw(backgroundColor);
	}
	if (drawFrame) {
		m_rect.drawFrame(0.0, frameThickness, frameColor);
	}
}

void UIRect::updateLayer() {
	UIComponent::updateLayer();

	m_rect = RoundRect(
		static_cast<int>(m_layer.left),
		static_cast<int>(m_layer.top),
		static_cast<int>(m_layer.width),
		static_cast<int>(m_layer.height),
		static_cast<int>(m_rect.r)
	);
}

void UIRect::updateMouseIntersection() {
	m_mouseLeftDown = m_rect.leftClicked();
	m_mouseLeftUp = m_rect.leftReleased();
	m_mouseLeftPress = m_rect.leftPressed();

	m_mouseRightDown = m_rect.rightClicked();
	m_mouseRightUp = m_rect.rightReleased();
	m_mouseRightPress = m_rect.rightPressed();

	m_preMouseOver = m_mouseOver;
	m_mouseOver = m_rect.mouseOver();
}
