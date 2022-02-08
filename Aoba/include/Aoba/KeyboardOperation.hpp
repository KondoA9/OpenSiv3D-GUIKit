#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba {
    // This class is used in UIInputField
    template <class T>
    class KeyboardOperation {
        struct State {
            bool down = false, pressed = false;

            void reset() {
                down = pressed = true;
            }
        };

        T m_key;
        std::function<void()> m_handler;
        State m_state;
        bool m_called = false;

        static int32 m_CursorMoveDuration;
        static Stopwatch m_CursorMoveDurationWatcher;

    public:
        KeyboardOperation(const T& key, const std::function<void()>& handler) : m_key(key), m_handler(handler) {}

        bool isCalled() const {
            return m_called;
        }

        State state() const {
            return m_state;
        }

        void callForce() {
            m_handler();
        }

        void update();

        bool tryCall();
    };
}
