#include <GUIKit/UICircle.h>

using namespace s3d::gui;

void UICircle::updateLayer() {
	UIComponent::updateLayer();
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

bool UICircle::mouseLeftDown() {
	if (!m_mouseRightDraggingEnable && m_circle.leftClicked() && m_circle.y >= 0) {
		focus();
		callMouseEventHandler(MouseEvent::LeftDown(this));
		m_mouseLeftDraggingEnable = true;
		return true;
	}
	return false;
}

bool UICircle::mouseLeftUp() {
	if (m_mouseLeftDraggingEnable && !m_mouseOver || m_circle.leftReleased()) {
		m_mouseLeftDraggingEnable = false;
		callMouseEventHandler(MouseEvent::LeftUp(this));
		return true;
	}
	return false;
}

bool UICircle::mouseLeftDragging() {
	if (m_circle.leftPressed()) {
		callMouseEventHandler(MouseEvent::LeftDragging(this));
		return true;
	}
	return false;
}

bool UICircle::mouseRightDown() {
	if (!m_mouseRightDraggingEnable && m_circle.rightClicked() && m_circle.y >= 0) {
		callMouseEventHandler(MouseEvent::RightDown(this));
		m_mouseRightDraggingEnable = true;
		return true;
	}
	return false;
}

bool UICircle::mouseRightUp() {
	if (m_mouseRightDraggingEnable && (!m_mouseOver || m_circle.rightReleased())) {
		m_mouseRightDraggingEnable = false;
		callMouseEventHandler(MouseEvent::RightUp(this));
		return true;
	}
	return false;
}

bool UICircle::mouseRightDragging() {
	if (m_mouseRightDraggingEnable && m_circle.rightPressed()) {
		callMouseEventHandler(MouseEvent::RightDragging(this));
		return true;
	}
	return false;
}

bool UICircle::mouseHovered() {
	static bool hovered = false;
	if (hovered && !m_mouseOver) {
		hovered = false;
	}
	else if (m_mouseOver) {
		callMouseEventHandler(MouseEvent::Hovered(this));
		hovered = true;
		return true;
	}
	return false;
}

bool UICircle::mouseHovering() {
	if (m_mouseOver) {
		callMouseEventHandler(MouseEvent::Hovering(this));
		return true;
	}
	return false;
}

bool UICircle::mouseUnHovered() {
	static bool hovered = false;
	if (hovered && !m_mouseOver) {
		if (m_mouseOver) {
			callMouseEventHandler(MouseEvent::UnHovered(this));
			return true;
		}
		hovered = false;
	}
	else if (m_mouseOver) {
		hovered = true;
	}
	return false;
}

bool UICircle::mouseWheel() {
	if (const double wheel = Mouse::Wheel(); m_mouseOver && wheel != 0.0) {
		callMouseEventHandler(MouseEvent::Wheel(this));
		return true;
	}
	return false;
}

void UICircle::update() {
	m_preMouseOver = m_mouseOver;
	m_mouseOver = m_circle.mouseOver();
}