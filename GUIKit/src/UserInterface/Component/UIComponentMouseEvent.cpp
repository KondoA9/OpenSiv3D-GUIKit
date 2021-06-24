#include <GUIKit/UIComponent.h>

using namespace s3d::gui;

bool UIComponent::mouseLeftDown() {
	if (!m_mouseLeftDraggingEnable && m_mouseLeftDown) {
		focus();
		callMouseEventHandler(MouseEvent::LeftDown(this));
		m_mouseLeftDraggingEnable = true;
		return true;
	}
	return false;
}

bool UIComponent::mouseLeftUp() {
	if (m_mouseLeftDraggingEnable && (!m_mouseOver || m_mouseLeftUp)) {
		m_mouseLeftDraggingEnable = false;
		callMouseEventHandler(MouseEvent::LeftUp(this));
		return true;
	}
	return false;
}

bool UIComponent::mouseLeftDragging() {
	if (m_mouseLeftDraggingEnable && m_mouseLeftPress) {
		callMouseEventHandler(MouseEvent::LeftDragging(this));
		return true;
	}
	return false;
}

bool UIComponent::mouseRightDown() {
	if (!m_mouseRightDraggingEnable && m_mouseRightDown) {
		callMouseEventHandler(MouseEvent::RightDown(this));
		m_mouseRightDraggingEnable = true;
		return true;
	}
	return false;
}

bool UIComponent::mouseRightUp() {
	if (m_mouseRightDraggingEnable && (!m_mouseOver || m_mouseRightUp)) {
		m_mouseRightDraggingEnable = false;
		callMouseEventHandler(MouseEvent::RightUp(this));
		return true;
	}
	return false;
}

bool UIComponent::mouseRightDragging() {
	if (m_mouseRightDraggingEnable && m_mouseRightPress) {
		callMouseEventHandler(MouseEvent::RightDragging(this));
		return true;
	}
	return false;
}

bool UIComponent::mouseHovered() {
	if (!m_preMouseOver && m_mouseOver) {
		callMouseEventHandler(MouseEvent::Hovered(this));
		return true;
	}
	return false;
}

bool UIComponent::mouseHovering() {
	if (m_mouseOver) {
		callMouseEventHandler(MouseEvent::Hovering(this));
		return true;
	}
	return false;
}

bool UIComponent::mouseUnHovered() {
	if (m_preMouseOver && !m_mouseOver) {
		callMouseEventHandler(MouseEvent::UnHovered(this));
		return true;
	}
	return false;
}

bool UIComponent::mouseWheel() {
	if (const double wheel = Mouse::Wheel(); m_mouseOver && wheel != 0.0) {
		callMouseEventHandler(MouseEvent::Wheel(this));
		return true;
	}
	return false;
}

void UIComponent::updateMouseEvents() {
	mouseLeftDown();
	mouseLeftUp();
	mouseLeftDragging();

	mouseRightDown();
	mouseRightUp();
	mouseRightDragging();

	mouseHovered();
	mouseHovering();
	mouseUnHovered();

	mouseWheel();
}

void UIComponent::ResetMouseEvents() {
	m_CallableMouseEvents.release();
}

void UIComponent::CallMouseEvents() {
	for (const auto& e : m_CallableMouseEvents) {
		for (const auto& handler : e.handlers) {
			handler.handler(e.mouseEvent);
		}
	}
}