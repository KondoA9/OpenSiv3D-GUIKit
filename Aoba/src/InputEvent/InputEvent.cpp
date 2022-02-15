#include "Aoba/InputEvent.hpp"

#include "Aoba/UIComponent.hpp"

namespace s3d::aoba {
    InputEvent::InputEvent(size_t _id, const UIComponent& _component, bool _callIfComponentInFront) :
        id(_id),
        componentId(_component.id()),
        wheel(Mouse::Wheel()),
        pos(Cursor::PosF()),
        previousPos(Cursor::PreviousPosF()),
        callIfComponentInFront(_callIfComponentInFront) {}
}
