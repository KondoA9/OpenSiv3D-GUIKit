#pragma once

#include <Siv3D.hpp>

#include "InputEvent.hpp"

// Raw events called when mouse down / up.
// Does not consider whether it is a long press event, dragging event or not
AobaCreateEventNSEvent(Mouse, LeftDownRaw)
AobaCreateEventNSEvent(Mouse, LeftUpRaw)

AobaCreateEventNSEvent(Mouse, RightDownRaw)
AobaCreateEventNSEvent(Mouse, RightUpRaw)

// Mouse down event is called if mouse up within 0.5s
AobaCreateEventNSEvent(Mouse, LeftDown)
AobaCreateEventNSEvent(Mouse, LeftDragging)
AobaCreateEventNSEvent(Mouse, LeftDraggingStart)
AobaCreateEventNSEvent(Mouse, LeftDraggingEnd)

AobaCreateEventNSEvent(Mouse, RightDown)
AobaCreateEventNSEvent(Mouse, RightDragging)
AobaCreateEventNSEvent(Mouse, RightDraggingStart)
AobaCreateEventNSEvent(Mouse, RightDraggingEnd)

// Hover events are called even if while mouse dragging
AobaCreateEventNSEvent(Mouse, Hovered)
AobaCreateEventNSEvent(Mouse, UnHovered)
AobaCreateEventNSEvent(Mouse, Hovering)

AobaCreateEventNSEvent(Mouse, Wheel)
