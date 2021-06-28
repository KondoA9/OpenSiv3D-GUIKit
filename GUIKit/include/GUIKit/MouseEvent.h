#pragma once

#include "IMouseEvent.h"

#include <Siv3D.hpp>

#define GUICreateInputEvent(Event) struct Event : public InputEvent { Event(UIComponent* _component) : InputEvent(typeid(Event).hash_code(), _component) {} };

namespace s3d::gui {
	namespace MouseEvent {
		GUICreateInputEvent(LeftDown);
		GUICreateInputEvent(LeftUp);
		GUICreateInputEvent(LeftDragging);

		GUICreateInputEvent(RightDown);
		GUICreateInputEvent(RightUp);
		GUICreateInputEvent(RightDragging);

		GUICreateInputEvent(Hovered);
		GUICreateInputEvent(UnHovered);
		GUICreateInputEvent(Hovering);

		GUICreateInputEvent(Wheel);
	}
}