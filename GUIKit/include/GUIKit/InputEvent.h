#pragma once

#include <Siv3D.hpp>

#define GUICreateInputEvent(Event) \
struct Event : public s3d::gui::InputEvent { \
	explicit Event(s3d::gui::UIComponent* _component, bool callIfComponentInFront = true) : \
		s3d::gui::InputEvent(typeid(Event).hash_code(), _component, callIfComponentInFront) \
	{} \
};

namespace s3d::gui {
	class UIComponent;

	struct InputEvent {
		const size_t id;
		const double wheel;
		const Vec2 pos, previousPos;
		const bool callIfComponentInFront;

		UIComponent* component;

		constexpr InputEvent(const InputEvent& e):
			id(e.id),
			wheel(e.wheel),
			pos(e.pos),
			previousPos(e.previousPos),
			callIfComponentInFront(e.callIfComponentInFront),
			component(e.component)
		{}

		InputEvent(size_t _id, UIComponent* _component, bool _callIfComponentInFront) :
			id(_id),
			wheel(Mouse::Wheel()),
			pos(Cursor::PosF()),
			previousPos(Cursor::PreviousPosF()),
			callIfComponentInFront(_callIfComponentInFront),
			component(_component)
		{}

		virtual ~InputEvent() = default;

		const InputEvent& operator =(const InputEvent& e) {
			assert(id == e.id);

			component = e.component;

			return *this;
		}
	};
}
