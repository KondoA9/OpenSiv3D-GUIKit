#pragma once

#include "Aoba/InputEventHandler.hpp"

namespace s3d::aoba {
    class InputEventManager {
        struct CallableInputEvent {
            InputEvent inputEvent;
            Array<InputEventHandler> handlers;

            CallableInputEvent(const InputEvent& _inputEvent, const Array<InputEventHandler>& _handlers) :
                inputEvent(_inputEvent), handlers(_handlers) {}
        };

    private:
        Array<CallableInputEvent> m_inputEvents;

    public:
        InputEventManager(const InputEventManager&) = delete;

        InputEventManager(InputEventManager&&) = delete;

        InputEventManager& operator=(const InputEventManager&) = delete;

        InputEventManager& operator=(InputEventManager&&) = delete;

        static void Register(const InputEvent& e);

        static void Unregister(size_t id);

        static void Call();

    private:
        InputEventManager() = default;

        ~InputEventManager() = default;

        static InputEventManager& Intance();
    };
}
