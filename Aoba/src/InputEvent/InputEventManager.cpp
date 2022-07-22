#include "src/InputEvent/InputEventManager.hpp"

#include "Aoba/UIComponent.hpp"
#include "src/ComponentStorage/ComponentStorage.hpp"

namespace s3d::aoba {
    InputEventManager& InputEventManager::Intance() {
        static InputEventManager instance;
        return instance;
    }

    void InputEventManager::Register(const InputEvent& e) {
        const auto& component = ComponentStorage::Get(e.componentId);
        auto& inputEvents     = Intance().m_inputEvents;

        // Get handlers that are matched to called event type
        const auto handlers = component.m_inputEventHandlers.removed_if(
            [e](const InputEventHandler& handler) { return handler.eventTypeId != e.id; });

        if (e.callIfComponentInFront && !component.penetrateMouseEvent) {
            if (inputEvents && inputEvents[inputEvents.size() - 1].inputEvent.componentId != e.componentId) {
                inputEvents = inputEvents.removed_if(
                    [](const CallableInputEvent& e) { return e.inputEvent.callIfComponentInFront; });
            }
            inputEvents.emplace_back(e, handlers);
        } else {
            // Append handlers if event stack is empty or the component penetrates a mouse event
            if (!inputEvents || component.penetrateMouseEvent) {
                inputEvents.emplace_back(e, handlers);
            } else {
                for (size_t i : step(inputEvents.size())) {
                    auto& behindComponentEvents = inputEvents[i];
                    if (behindComponentEvents.inputEvent.id == e.id
                        && behindComponentEvents.inputEvent.callIfComponentInFront) {
                        behindComponentEvents.handlers = handlers;
                        break;
                    }
                    // Append handler if a event that is same type of the event does not exists
                    else if (i == inputEvents.size() - 1) {
                        inputEvents.emplace_back(e, handlers);
                    }
                }
            }
        }
    }

    void InputEventManager::Unregister(size_t id) {
        if (Intance().m_inputEvents) {
            Intance().m_inputEvents.remove_if(
                [id](const CallableInputEvent& e) { return e.inputEvent.componentId == id; });
        }
    }

    void InputEventManager::Call() {
        UIComponent::UpdateFocusEvent();

        // Copy events to prevent outbreak of incompatible vector caused by calling events within events
        const Array<CallableInputEvent> events = Intance().m_inputEvents;
        Intance().m_inputEvents.release();

        for (const auto& e : events) {
            if (ComponentStorage::Has(e.inputEvent.componentId)) {
                for (const auto& handler : e.handlers) {
                    handler.handler(e.inputEvent);
                }
            }
        }
    }
}
