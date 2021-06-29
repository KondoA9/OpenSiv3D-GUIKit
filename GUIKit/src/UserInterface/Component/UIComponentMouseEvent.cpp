#include <GUIKit/UIComponent.h>

using namespace s3d::gui;

void UIComponent::updateInputEvents() {
	// Call raw event
	if (m_mouseLeftDown) {
		callInputEventHandler(MouseEvent::LeftDownRaw(this));
	}
	if (m_mouseRightDown) {
		callInputEventHandler(MouseEvent::RightDownRaw(this));
	}
	if (m_mouseLeftUp) {
		callInputEventHandler(MouseEvent::LeftUpRaw(this));
	}
	if (m_mouseRightUp) {
		callInputEventHandler(MouseEvent::RightUpRaw(this));
	}

	// Prepare to call mouse event
	if (m_mouseLeftDown || m_mouseRightDown) {
		m_clickedPos = Cursor::PosF();
		m_clickIntervalTimer = 0.0;
		m_mouseDownEnable = true;
	}

	// Increase timer
	if (m_mouseDownEnable) {
		m_clickIntervalTimer += Scene::DeltaTime();
		// If timer > 0.5s, click event will not be called
		if (m_clickIntervalTimer > 0.5) {
			m_mouseDownEnable = false;
		}
	}

	// Mouse dragging start
	if (!m_mouseDragging && (m_mouseLeftPress || m_mouseRightPress)) {
		if (m_clickedPos.distanceFrom(Cursor::PosF()) > 10.0) {
			if (m_mouseLeftPress) {
				focus();
				callInputEventHandler(MouseEvent::LeftDraggingStart(this));
			}
			else {
				callInputEventHandler(MouseEvent::RightDraggingStart(this));
			}
			m_mouseDownEnable = false;
			m_mouseDragging = true;
		}
	}

	// Mouse down event
	if (m_mouseDownEnable && (m_mouseLeftUp || m_mouseRightUp)) {
		if (m_mouseLeftUp) {
			focus();
			callInputEventHandler(MouseEvent::LeftDown(this));
		}
		else {
			callInputEventHandler(MouseEvent::RightDown(this));
		}
		m_mouseDownEnable = false;
	}

	// Mouse dragging and end dragging event
	if (m_mouseDragging) {
		if (m_mouseLeftPress) {
			callInputEventHandler(MouseEvent::LeftDragging(this));
		}
		if (m_mouseLeftUp) {
			callInputEventHandler(MouseEvent::LeftDraggingEnd(this));
			m_mouseDragging = false;
		}
		if (m_mouseRightPress) {
			callInputEventHandler(MouseEvent::RightDragging(this));
		}
		if (m_mouseRightUp) {
			callInputEventHandler(MouseEvent::RightDraggingEnd(this));
			m_mouseDragging = false;
		}
	}

	// Mouse hover event
	if (!m_preMouseOver && m_mouseOver) {
		callInputEventHandler(MouseEvent::Hovered(this));
	}

	if (m_mouseOver) {
		callInputEventHandler(MouseEvent::Hovering(this));
	}

	if (m_preMouseOver && !m_mouseOver) {
		callInputEventHandler(MouseEvent::UnHovered(this));
	}

	// Mouse wheel event
	if (const double wheel = Mouse::Wheel(); m_mouseOver && wheel != 0.0) {
		callInputEventHandler(MouseEvent::Wheel(this));
	}
}

void UIComponent::ResetInputEvents() {
	m_CallableInputEvents.release();
}

void UIComponent::CallInputEvents() {
	for (const auto& e : m_CallableInputEvents) {
		for (const auto& handler : e.handlers) {
			handler.handler(e.mouseEvent);
		}
	}
}