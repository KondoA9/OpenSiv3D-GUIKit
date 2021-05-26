#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIComponent;

	enum class MouseEventType : size_t {
		Clicked,
		Hovered,
		Hovering,
		UnHovered,
		Dragging,
		Wheel
	};

	struct MouseEvent {
		const MouseEventType type;
		const double wheel;
		const Vec2 pos;
		UIComponent* component = nullptr;

		MouseEvent(MouseEventType _type, UIComponent* _component) :
			type(_type),
			component(_component),
			wheel(Mouse::Wheel()),
			pos(Cursor::PosF())
		{}
	};

	class MouseEventHandler {
	public:
		MouseEventType eventType;
		std::function<void(const MouseEvent&)> handler;

		MouseEventHandler(MouseEventType _eventType, const std::function<void(MouseEvent)>& _handler) :
			eventType(_eventType),
			handler(_handler)
		{}
	};
}