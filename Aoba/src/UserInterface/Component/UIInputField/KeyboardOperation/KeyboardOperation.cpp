#include "Aoba/KeyboardOperation.hpp"

namespace s3d::aoba {
    template <class T>
    int32 KeyboardOperation<T>::m_CursorMoveDuration = 500;
    template <class T>
    Stopwatch KeyboardOperation<T>::m_CursorMoveDurationWatcher;

    template <class T>
    void KeyboardOperation<T>::update() {
        m_called = false;
        m_state.reset();

        m_state.down    = m_key.down();
        m_state.pressed = m_key.pressed() && m_CursorMoveDurationWatcher.ms() > m_CursorMoveDuration;

        if (m_state.down) {
            m_CursorMoveDuration = 500;
            m_CursorMoveDurationWatcher.restart();
        } else if (m_state.pressed) {
            m_CursorMoveDuration = 30;
            m_CursorMoveDurationWatcher.restart();
        }
    }

    template <class T>
    bool KeyboardOperation<T>::tryCall() {
        if (m_state.down | m_state.pressed) {
            m_handler();
            m_called = true;
        }
        return m_called;
    }

    template void KeyboardOperation<Input>::update();
    template void KeyboardOperation<InputGroup>::update();
    template void KeyboardOperation<InputCombination>::update();

    template bool KeyboardOperation<Input>::tryCall();
    template bool KeyboardOperation<InputGroup>::tryCall();
    template bool KeyboardOperation<InputCombination>::tryCall();
}
