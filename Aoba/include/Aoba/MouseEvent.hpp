#pragma once

#include <Siv3D.hpp>

#include "InputEvent.hpp"

namespace s3d::aoba {
	namespace MouseEvent {
		// Raw events called when mouse down / up.
		// Does not consider whether it is a long press event, dragging event or not
		AobaCreateInputEvent(LeftDownRaw);
		AobaCreateInputEvent(LeftUpRaw);

		AobaCreateInputEvent(RightDownRaw);
		AobaCreateInputEvent(RightUpRaw);

		// Mouse down event is called if mouse up within 0.5s
		AobaCreateInputEvent(LeftDown);
		AobaCreateInputEvent(LeftDragging);
		AobaCreateInputEvent(LeftDraggingStart);
		AobaCreateInputEvent(LeftDraggingEnd);

		AobaCreateInputEvent(RightDown);
		AobaCreateInputEvent(RightDragging);
		AobaCreateInputEvent(RightDraggingStart);
		AobaCreateInputEvent(RightDraggingEnd);

		// Hover events are called even if while mouse dragging
		AobaCreateInputEvent(Hovered);
		AobaCreateInputEvent(UnHovered);
		AobaCreateInputEvent(Hovering);

		AobaCreateInputEvent(Wheel);
	}
}
