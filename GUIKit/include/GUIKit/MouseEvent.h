#pragma once

#include "IMouseEvent.h"

#include <Siv3D.hpp>

#define GUICreateMouseEvent(Event) struct Event : public IMouseEvent { Event(UIComponent* _component) : IMouseEvent(typeid(Event).hash_code(), _component) {} };

namespace s3d::gui {
	namespace MouseEvent {
		GUICreateMouseEvent(LeftDown);
		GUICreateMouseEvent(LeftUp);
		GUICreateMouseEvent(LeftDragging);

		GUICreateMouseEvent(RightDown);
		GUICreateMouseEvent(RightUp);
		GUICreateMouseEvent(RightDragging);

		GUICreateMouseEvent(Hovered);
		GUICreateMouseEvent(UnHovered);
		GUICreateMouseEvent(Hovering);

		GUICreateMouseEvent(Wheel);
	}
}