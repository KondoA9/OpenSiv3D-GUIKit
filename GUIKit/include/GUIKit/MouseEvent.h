#pragma once

#include "InputEvent.h"

#include <Siv3D.hpp>

#define GUICreateInputEvent(Event) struct Event : public InputEvent { Event(UIComponent* _component) : InputEvent(typeid(Event).hash_code(), _component) {} };

namespace s3d::gui {
	namespace MouseEvent {
		// Raw events called when mouse down / up.
		// Does not consider whether it is a long press event, dragging event or not
		GUICreateInputEvent(LeftDownRaw);
		GUICreateInputEvent(LeftUpRaw);

		GUICreateInputEvent(RightDownRaw);
		GUICreateInputEvent(RightUpRaw);

		// Mouse down event is called if mouse up within 0.5s
		GUICreateInputEvent(LeftDown);
		GUICreateInputEvent(LeftDragging);
		GUICreateInputEvent(LeftDraggingStart);
		GUICreateInputEvent(LeftDraggingEnd);

		GUICreateInputEvent(RightDown);
		GUICreateInputEvent(RightDragging);
		GUICreateInputEvent(RightDraggingStart);
		GUICreateInputEvent(RightDraggingEnd);

		// Hover events are called even if while mouse dragging
		GUICreateInputEvent(Hovered);
		GUICreateInputEvent(UnHovered);
		GUICreateInputEvent(Hovering);

		GUICreateInputEvent(Wheel);
	}
}