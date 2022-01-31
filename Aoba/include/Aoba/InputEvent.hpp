#pragma once

#include <Siv3D.hpp>

#define AobaCreateInputEvent(Event) \
struct Event : public s3d::aoba::InputEvent { \
	explicit Event(s3d::aoba::UIComponent* _component, bool callIfComponentInFront = true) : \
		s3d::aoba::InputEvent(typeid(Event).hash_code(), _component, callIfComponentInFront) \
	{} \
};

namespace s3d::aoba {
	class UIComponent;

	struct InputEvent {
		const size_t id;
		const double wheel;
		const Vec2 pos, previousPos;
		const bool callIfComponentInFront;

		UIComponent* component;

		InputEvent() = delete;

		InputEvent(size_t _id, UIComponent* _component, bool _callIfComponentInFront) noexcept :
			id(_id),
			wheel(Mouse::Wheel()),
			pos(Cursor::PosF()),
			previousPos(Cursor::PreviousPosF()),
			callIfComponentInFront(_callIfComponentInFront),
			component(_component)
		{}

		virtual ~InputEvent() = default;

		constexpr InputEvent(const InputEvent& e) noexcept = default;

		constexpr InputEvent(InputEvent&& e) noexcept = default;

		const InputEvent& operator =(const InputEvent& e) noexcept {
			assert(id == e.id);

			component = e.component;

			return *this;
		}

		const InputEvent& operator =(InputEvent&& e) noexcept {
			assert(id == e.id);

			component = std::move(e.component);

			return *this;
		}
	};
}
