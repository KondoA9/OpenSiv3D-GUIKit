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

	// compile fails if class T is not Input, InputCombination or InputGroup 
    template <class T>
    class KeyShortcut : public KeyShortcutBase {
    private:
        const T m_input;

    public:
        KeyShortcut(const T& input, const std::function<void()>& callback) :
            KeyShortcutBase(callback), m_input(input) {}

        bool keyDown() const override;
    };
}
