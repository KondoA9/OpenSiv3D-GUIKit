#pragma once

#include "IMouseEvent.h"

namespace s3d::gui {
	class InputEventHandler {
	public:
		size_t eventTypeId;
		std::function<void(const InputEvent&)> handler;

		InputEventHandler(const std::function<void(InputEvent)>& _handler) :
			handler(_handler)
		{}

		template<class T>
		void setEvent() {
			eventTypeId = typeid(T).hash_code();
		}
	};
}