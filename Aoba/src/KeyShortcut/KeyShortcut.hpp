#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba {
    class KeyShortcutBase {
    private:
        const std::function<void()> m_callback;

    protected:
        explicit KeyShortcutBase(const std::function<void()>& callback) : m_callback(callback) {}

    public:
        virtual ~KeyShortcutBase() {}

        virtual bool keyDown() const = 0;

        void call() {
            m_callback();
        }
    };

    // T must be s3d::Input, s3d::InputCombination or s3d::InputGroup.
    template <class T,
              typename =
                  typename std::enable_if_t<std::is_same<T, Input>::value || std::is_same<T, InputCombination>::value
                                            || std::is_same<T, InputGroup>::value>>
    class KeyShortcut final : public KeyShortcutBase {
    private:
        const T m_input;

    public:
        KeyShortcut(const T& input, const std::function<void()>& callback) :
            KeyShortcutBase(callback), m_input(input) {}

        bool keyDown() const override {
            return m_input.down();
        }
    };
}
