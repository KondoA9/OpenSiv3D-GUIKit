#include "UIRect.h"

using namespace s3d::gui;

void UIRect::updateLayer() {
	UIComponent::updateLayer();
	m_rect = Rect(
		static_cast<int>(m_layer.left.value),
		static_cast<int>(m_layer.top.value),
		static_cast<int>(m_layer.width.value),
		static_cast<int>(m_layer.height.value)
	);
}

bool UIRect::mouseClicked() {
	if (!m_mouseDraggingEnable && m_rect.leftClicked()) {
		callMouseEventHandler(MouseEvent(MouseEventType::Clicked, this));
		m_mouseDraggingEnable = true;
		return true;
	}
	return false;
}

bool UIRect::mouseUp() {
	if (m_mouseDraggingEnable && (!m_mouseOver || m_rect.leftReleased())) {
		m_mouseDraggingEnable = false;
		return true;
	}
	return false;
}

bool UIRect::mouseHovered() {
	if (!m_preMouseOver && m_mouseOver) {
		callMouseEventHandler(MouseEvent(MouseEventType::Hovered, this));
		return true;
	}
	return false;
}

bool UIRect::mouseHovering() {
	if (m_mouseOver) {
		callMouseEventHandler(MouseEvent(MouseEventType::Hovering, this));
		return true;
	}
	return false;
}

bool UIRect::mouseUnHovered() {
	if (m_preMouseOver && !m_mouseOver) {
		callMouseEventHandler(MouseEvent(MouseEventType::UnHovered, this));
		return true;
	}
	return false;
}

bool UIRect::mouseDragging() {
	if (m_mouseDraggingEnable && m_rect.leftPressed()) {
		callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
		return true;
	}
	return false;
}

bool UIRect::mouseWheel() {
	if (const double wheel = Mouse::Wheel(); m_mouseOver && wheel != 0.0) {
		callMouseEventHandler(MouseEvent(MouseEventType::Wheel, this));
		return true;
	}
	return false;
}

void UIRect::updateMouseEvent() {
	m_preMouseOver = m_mouseOver;
	m_mouseOver = m_rect.mouseOver();
	UIComponent::updateMouseEvent();
}