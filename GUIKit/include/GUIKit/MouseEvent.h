#pragma once

#include <Siv3D.hpp>

#define GUICreateMouseEvent(Event) struct Event : public IMouseEvent { using IMouseEvent::IMouseEvent; };

namespace s3d::gui {
	class UIComponent;

	/*enum class MouseEventType : size_t {
		LeftDown,
		LeftUp,
		LeftDragging,
		RightDown,
		RightUp,
		RightDragging,
		Hovered,
		Hovering,
		UnHovered,
		Wheel
	};

	struct MouseEvent {
		MouseEventType type;
		double wheel;
		Vec2 pos;
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
	};*/

	struct IMouseEvent {
		UIComponent* component = nullptr;
		double wheel;
		Vec2 pos;

		IMouseEvent(UIComponent* _component) :
			component(_component),
			wheel(Mouse::Wheel()),
			pos(Cursor::PosF())
		{}

		virtual ~IMouseEvent() {}
	};

	class MouseEventHandler {
	public:
		size_t eventType;
		std::function<void(const IMouseEvent&)> handler;

		MouseEventHandler(const std::function<void(IMouseEvent)>& _handler) :
			handler(_handler)
		{}

		template<class T>
		void setEvent() {
			eventType = typeid(T).hash_code();
		}

		template<class T>
		bool compare() const {
			return eventType == typeid(T).hash_code();
		}
	};

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