#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIComponent;

	struct InputEvent {
		const size_t id;
		const double wheel;
		const Vec2 pos, previousPos;

		UIComponent* component = nullptr;

		InputEvent(size_t _id, UIComponent* _component) :
			id(_id),
			component(_component),
			wheel(Mouse::Wheel()),
			pos(Cursor::PosF()),
			previousPos(Cursor::PreviousPosF())
		{}

		virtual ~InputEvent() = default;

		InputEvent operator =(const InputEvent& e) {
			assert(id == e.id);

			component = e.component;

			return *this;
		}
	};
}