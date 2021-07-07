#include <GUIKit/UIComponent.h>

using namespace s3d::gui;

void UIComponent::updateInputEvents() {
	// Fix mouse status
	{
		const bool inDrawableRegion = m_drawableRegion.mouseOver();
		m_mouseOver &= inDrawableRegion;

		m_mouseLeftDown &= inDrawableRegion;
		m_mouseLeftPress &= inDrawableRegion;
		m_mouseLeftUp &= inDrawableRegion;

		m_mouseRightDown &= inDrawableRegion;
		m_mouseRightPress &= inDrawableRegion;
		m_mouseRightUp &= inDrawableRegion;
	}

	// Call raw events
	if (m_mouseLeftDown) {
		registerInputEvent(MouseEvent::LeftDownRaw(this));
	}
	if (m_mouseRightDown) {
		registerInputEvent(MouseEvent::RightDownRaw(this));
	}
	if (m_mouseLeftUp) {
		registerInputEvent(MouseEvent::LeftUpRaw(this));
	}
	if (m_mouseRightUp) {
		registerInputEvent(MouseEvent::RightUpRaw(this));
	}

	// Prepare to call mouse event
	if (m_mouseLeftDown || m_mouseRightDown) {
		m_clickedPos = Cursor::PosF();
		m_clickIntervalTimer = 0.0;
		m_mouseDownEnable = true;
		m_mouseDownRaw = true;
	}
	if (m_mouseLeftUp || m_mouseRightUp) {
		m_mouseDownRaw = false;
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
	if (!m_mouseDragging && m_mouseDownRaw && (m_mouseLeftPress || m_mouseRightPress)) {
		if (m_clickedPos.distanceFrom(Cursor::PosF()) > 10.0) {
			if (m_mouseLeftPress) {
				focus();
				registerInputEvent(MouseEvent::LeftDraggingStart(this));
			}
			else {
				registerInputEvent(MouseEvent::RightDraggingStart(this));
			}
			m_mouseDownEnable = false;
			m_mouseDragging = true;
		}
	}

	// Mouse down event
	if (m_mouseDownEnable && (m_mouseLeftUp || m_mouseRightUp)) {
		if (m_mouseLeftUp) {
			focus();
			registerInputEvent(MouseEvent::LeftDown(this));
		}
		else {
			registerInputEvent(MouseEvent::RightDown(this));
		}
		m_mouseDownEnable = false;
	}

	// Mouse dragging and end dragging event
	if (m_mouseDragging) {
		if (m_mouseLeftPress) {
			registerInputEvent(MouseEvent::LeftDragging(this));
		}
		if (m_mouseLeftUp) {
			registerInputEvent(MouseEvent::LeftDraggingEnd(this));
			m_mouseDragging = false;
		}
		if (m_mouseRightPress) {
			registerInputEvent(MouseEvent::RightDragging(this));
		}
		if (m_mouseRightUp) {
			registerInputEvent(MouseEvent::RightDraggingEnd(this));
			m_mouseDragging = false;
		}
	}

	// Mouse hover event
	if (!m_preMouseOver && m_mouseOver) {
		registerInputEvent(MouseEvent::Hovered(this));
	}

	if (m_mouseOver) {
		registerInputEvent(MouseEvent::Hovering(this));
	}

	if (m_preMouseOver && !m_mouseOver) {
		registerInputEvent(MouseEvent::UnHovered(this, false));
		m_mouseDownRaw = false;
	}

	// Mouse wheel event
	if (const double wheel = Mouse::Wheel(); m_mouseOver && wheel != 0.0) {
		registerInputEvent(MouseEvent::Wheel(this));
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
