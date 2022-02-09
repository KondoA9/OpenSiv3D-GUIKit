#include "Aoba/Component/UIBase.hpp"

namespace s3d::aoba {
    void UIBase::updateInputEvents() {
        // Fix mouse status
        m_mouseCondition &= m_drawableRegion.mouseOver();

        // Call raw events
        if (m_mouseCondition.left.down) {
            registerInputEvent(Event::Mouse::LeftDownRaw(this));
        }
        if (m_mouseCondition.right.down) {
            registerInputEvent(Event::Mouse::RightDownRaw(this));
        }
        if (m_mouseCondition.left.up) {
            registerInputEvent(Event::Mouse::LeftUpRaw(this));
        }
        if (m_mouseCondition.right.up) {
            registerInputEvent(Event::Mouse::RightUpRaw(this));
        }

        // Prepare to call mouse event
        if (m_mouseCondition.left.down || m_mouseCondition.right.down) {
            m_clickedPos         = Cursor::PosF();
            m_clickIntervalTimer = 0.0;
            m_mouseDownEnable    = true;
            m_mouseDownRaw       = true;
        }
        if (m_mouseCondition.left.up || m_mouseCondition.right.up) {
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
        if (!m_mouseDragging && m_mouseDownRaw && (m_mouseCondition.left.press || m_mouseCondition.right.press)) {
            if (m_clickedPos.distanceFrom(Cursor::PosF()) > 10.0) {
                if (m_mouseCondition.left.press) {
                    focus();
                    registerInputEvent(Event::Mouse::LeftDraggingStart(this));
                } else {
                    registerInputEvent(Event::Mouse::RightDraggingStart(this));
                }
                m_mouseDownEnable = false;
                m_mouseDragging   = true;
            }
        }

        // Mouse down event
        if (m_mouseDownEnable && (m_mouseCondition.left.up || m_mouseCondition.right.up)) {
            if (m_mouseCondition.left.up) {
                focus();
                registerInputEvent(Event::Mouse::LeftDown(this));
            } else {
                registerInputEvent(Event::Mouse::RightDown(this));
            }
            m_mouseDownEnable = false;
        }

        // Mouse dragging and end dragging event
        if (m_mouseDragging) {
            if (m_mouseCondition.left.press) {
                registerInputEvent(Event::Mouse::LeftDragging(this));
            }
            if (m_mouseCondition.left.up) {
                registerInputEvent(Event::Mouse::LeftDraggingEnd(this));
                m_mouseDragging = false;
            }
            if (m_mouseCondition.right.press) {
                registerInputEvent(Event::Mouse::RightDragging(this));
            }
            if (m_mouseCondition.right.up) {
                registerInputEvent(Event::Mouse::RightDraggingEnd(this));
                m_mouseDragging = false;
            }
        }

        // Mouse hover event
        if (!m_mouseCondition.preHover && m_mouseCondition.hover) {
            registerInputEvent(Event::Mouse::Hovered(this));
        }

        if (m_mouseCondition.hover) {
            registerInputEvent(Event::Mouse::Hovering(this));
        }

        if (m_mouseCondition.preHover && !m_mouseCondition.hover) {
            registerInputEvent(Event::Mouse::UnHovered(this, false));
            m_mouseDownRaw = false;
        }

        // Mouse wheel event
        if (const double wheel = Mouse::Wheel(); m_mouseCondition.hover && wheel != 0.0) {
            registerInputEvent(Event::Mouse::Wheel(this));
        }
    }

    void UIBase::UpdateFocusEvent() {
        const bool noFocusedComponent = !m_FocusedComponent && !m_PreviousFocusedComponent;
        const bool noFocusEventCalled = m_FocusedComponent && m_PreviousFocusedComponent
                                        && m_PreviousFocusedComponent->id() == m_FocusedComponent->id();

        if (!noFocusedComponent && !noFocusEventCalled) {
            if (m_FocusedComponent && m_PreviousFocusedComponent) {
                m_PreviousFocusedComponent->registerInputEvent(
                    Event::Component::UnFocused(m_PreviousFocusedComponent.get(), false));
                m_FocusedComponent->registerInputEvent(Event::Component::Focused(m_FocusedComponent.get(), false));
            } else if (m_FocusedComponent) {
                m_FocusedComponent->registerInputEvent(Event::Component::Focused(m_FocusedComponent.get(), false));
            } else if (m_PreviousFocusedComponent) {
                m_PreviousFocusedComponent->registerInputEvent(
                    Event::Component::UnFocused(m_PreviousFocusedComponent.get(), false));
            }
        }

        m_PreviousFocusedComponent = m_FocusedComponent;
    }

    void UIBase::CallInputEvents() {
        UpdateFocusEvent();

        // Copy events to prevent outbreak of incompatible vector caused by calling events within events
        const Array<CallableInputEvent> events = m_CallableInputEvents;
        m_CallableInputEvents.release();

        for (const auto& e : events) {
            for (const auto& handler : e.handlers) {
                handler.handler(e.mouseEvent);
            }
        }
    }

    void UIBase::_updateMouseCondition(
        bool leftDown, bool leftUp, bool leftPress, bool rightDown, bool rightUp, bool rightPress, bool hover) {
        m_mouseCondition.left.down  = leftDown;
        m_mouseCondition.left.up    = leftUp;
        m_mouseCondition.left.press = leftPress;

        m_mouseCondition.right.down  = rightDown;
        m_mouseCondition.right.up    = rightUp;
        m_mouseCondition.right.press = rightPress;

        m_mouseCondition.preHover = m_mouseCondition.hover;
        m_mouseCondition.hover    = hover;
    }
}
