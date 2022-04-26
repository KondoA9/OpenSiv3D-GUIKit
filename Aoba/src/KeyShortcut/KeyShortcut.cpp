#include "KeyShortcut.hpp"

namespace s3d::aoba {
    template <>
    bool KeyShortcut<Input>::keyDown() const {
        return m_input.down();
    }

    template <>
    bool KeyShortcut<InputCombination>::keyDown() const {
        return m_input.down();
    }

    template <>
    bool KeyShortcut<InputGroup>::keyDown() const {
        return m_input.down();
    }
}
