#include "UIRect.h"

using namespace s3d::gui;

void UIRect::draw() {
	if (!drawable()) {
		return;
	}

	if (fillInner) {
		m_rect.draw(backgroundColor);
	}
	if (drawFrame) {
		m_rect.drawFrame(0.0, frameThickness, frameColor);
	}
}

void UIRect::updateLayer() {
	UIComponent::updateLayer();
	m_rect = Rect(
		static_cast<int>(m_layer.left.value),
		static_cast<int>(m_layer.top.value),
		static_cast<int>(m_layer.width.value),
		static_cast<int>(m_layer.height.value)
	);
}

bool UIRect::mouseLeftDown() {
	if (!m_mouseLeftDraggingEnable && m_rect.leftClicked() && m_rect.y >= 0) {
		callMouseEventHandler(MouseEvent(MouseEventType::LeftDown, this));
		m_mouseLeftDraggingEnable = true;
		return true;
	}
	return false;
}

bool UIRect::mouseLeftUp() {
	if (m_mouseLeftDraggingEnable && (!m_mouseOver || m_rect.leftReleased())) {
		m_mouseLeftDraggingEnable = false;
		callMouseEventHandler(MouseEvent(MouseEventType::LeftUp, this));
		return true;
	}
	return false;
}

bool UIRect::mouseLeftDragging() {
	if (m_mouseLeftDraggingEnable && m_rect.leftPressed()) {
		callMouseEventHandler(MouseEvent(MouseEventType::LeftDragging, this));
		return true;
	}
	return false;
}

bool UIRect::mouseRightDown() {
	if (!m_mouseRightDraggingEnable && m_rect.rightClicked() && m_rect.y >= 0) {
		callMouseEventHandler(MouseEvent(MouseEventType::RightDown, this));
		m_mouseRightDraggingEnable = true;
		return true;
	}
	return false;
}

bool UIRect::mouseRightUp() {
	if (m_mouseRightDraggingEnable && (!m_mouseOver || m_rect.rightReleased())) {
		m_mouseRightDraggingEnable = false;
		callMouseEventHandler(MouseEvent(MouseEventType::RightUp, this));
		return true;
	}
	return false;
}

bool UIRect::mouseRightDragging() {
	if (m_mouseRightDraggingEnable && m_rect.rightPressed()) {
		callMouseEventHandler(MouseEvent(MouseEventType::RightDragging, this));
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