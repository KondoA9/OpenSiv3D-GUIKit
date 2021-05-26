#include "UICircle.h"

using namespace s3d::gui;

void UICircle::updateLayer() {
	UIComponent::updateLayer();
	m_circle = Circle(Arg::center(static_cast<int>(m_layer.centerX.value), static_cast<int>(m_layer.centerY.value)), (m_layer.height.value + m_layer.width.value) * 0.25);
}

void UICircle::draw() {
	UIComponent::draw();
	m_circle.draw(backgroundColor);
}

bool UICircle::mouseDown() {
	if (m_circle.leftClicked()) {
		callMouseEventHandler(MouseEvent(MouseEventType::Down, this));
		m_mouseDraggingEnable = true;
		return true;
	}
	return false;
}

bool UICircle::mouseUp() {
	if (m_mouseDraggingEnable && !m_mouseOver || m_circle.leftReleased()) {
		m_mouseDraggingEnable = false;
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
		callMouseEventHandler(MouseEvent(MouseEventType::Hovered, this));
		hovered = true;
		return true;
	}
	return false;
}

bool UICircle::mouseHovering() {
	if (m_mouseOver) {
		callMouseEventHandler(MouseEvent(MouseEventType::Hovering, this));
		return true;
	}
	return false;
}

bool UICircle::mouseUnHovered() {
	static bool hovered = false;
	if (hovered && !m_mouseOver) {
		if (m_mouseOver) {
			callMouseEventHandler(MouseEvent(MouseEventType::UnHovered, this));
			return true;
		}
		hovered = false;
	}
	else if (m_mouseOver) {
		hovered = true;
	}
	return false;
}

bool UICircle::mouseDragging() {
	if (m_circle.leftPressed()) {
		callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
		return true;
	}
	return false;
}

bool UICircle::mouseWheel() {
	if (const double wheel = Mouse::Wheel(); m_mouseOver && wheel != 0.0) {
		callMouseEventHandler(MouseEvent(MouseEventType::Wheel, this));
		return true;
	}
	return false;
}

void UICircle::updateMouseEvent() {
	m_mouseOver = m_circle.mouseOver();
	UIComponent::updateMouseEvent();
}