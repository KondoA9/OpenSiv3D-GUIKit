#include "Aoba/UIComponent.hpp"
#include "src/ComponentStorage/ComponentStorage.hpp"
#include "src/InputEvent/InputEventManager.hpp"

namespace s3d::aoba {
    void UIComponent::registerInputEvent(const InputEvent& e) {
        InputEventManager::Register(e);
    }

    void UIComponent::updateInputEvents() {
        // Fix mouse status
        m_mouseCondition &= m_drawableRegion.mouseOver();

        // Call raw events
        if (m_mouseCondition.left.down) {
            InputEventManager::Register(Event::Mouse::LeftDownRaw(this));
        }
        if (m_mouseCondition.right.down) {
            InputEventManager::Register(Event::Mouse::RightDownRaw(this));
        }
        if (m_mouseCondition.left.up) {
            InputEventManager::Register(Event::Mouse::LeftUpRaw(this));
        }
        if (m_mouseCondition.right.up) {
            InputEventManager::Register(Event::Mouse::RightUpRaw(this));
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
                    InputEventManager::Register(Event::Mouse::LeftDraggingStart(this));
                } else {
                    InputEventManager::Register(Event::Mouse::RightDraggingStart(this));
                }
                m_mouseDownEnable = false;
                m_mouseDragging   = true;
            }
        }

        // Mouse down event
        if (m_mouseDownEnable && (m_mouseCondition.left.up || m_mouseCondition.right.up)) {
            if (m_mouseCondition.left.up) {
                focus();
                InputEventManager::Register(Event::Mouse::LeftDown(this));
            } else {
                InputEventManager::Register(Event::Mouse::RightDown(this));
            }
            m_mouseDownEnable = false;
        }

        // Mouse dragging and end dragging event
        if (m_mouseDragging) {
            if (m_mouseCondition.left.press) {
                InputEventManager::Register(Event::Mouse::LeftDragging(this));
            }
            if (m_mouseCondition.left.up) {
                InputEventManager::Register(Event::Mouse::LeftDraggingEnd(this));
                m_mouseDragging = false;
            }
            if (m_mouseCondition.right.press) {
                InputEventManager::Register(Event::Mouse::RightDragging(this));
            }
            if (m_mouseCondition.right.up) {
                InputEventManager::Register(Event::Mouse::RightDraggingEnd(this));
                m_mouseDragging = false;
            }
        }

        // Mouse hover event
        if (!m_mouseCondition.preHover && m_mouseCondition.hover) {
            InputEventManager::Register(Event::Mouse::Hovered(this));
        }

        if (m_mouseCondition.hover) {
            InputEventManager::Register(Event::Mouse::Hovering(this));
        }

        if (m_mouseCondition.preHover && !m_mouseCondition.hover) {
            InputEventManager::Register(Event::Mouse::UnHovered(this, false));
            m_mouseDownRaw = false;
        }

        // Mouse wheel event
        if (const double wheel = Mouse::Wheel(); m_mouseCondition.hover && wheel != 0.0) {
            InputEventManager::Register(Event::Mouse::Wheel(this));
        }
    }

    void UIComponent::UpdateFocusEvent() {
        const bool noFocusedComponent = !m_FocusedComponentId && !m_PreviousFocusedComponentId;
        const bool noFocusEventCalled = m_FocusedComponentId && m_PreviousFocusedComponentId
                                        && m_FocusedComponentId == m_PreviousFocusedComponentId;

        if (!noFocusedComponent && !noFocusEventCalled) {
            if (m_PreviousFocusedComponentId && ComponentStorage::Has(m_PreviousFocusedComponentId.value())) {
                InputEventManager::Register(Event::Component::UnFocused(
                    ComponentStorage::Get(m_PreviousFocusedComponentId.value()), false));
            }
            if (m_FocusedComponentId && ComponentStorage::Has(m_FocusedComponentId.value())) {
                InputEventManager::Register(
                    Event::Component::Focused(ComponentStorage::Get(m_FocusedComponentId.value()), false));
            }
        }

        m_PreviousFocusedComponentId = m_FocusedComponentId;
    }

    void UIComponent::_updateMouseCondition(bool leftDown,
                                            bool leftUp,
                                            bool leftPress,
                                            bool rightDown,
                                            bool rightUp,
                                            bool rightPress,
                                            bool hover) noexcept {
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
