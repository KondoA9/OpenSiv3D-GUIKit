#pragma once

#include "InputEvent.h"

namespace s3d::gui {
	class InputEventHandler {
	public:
		size_t eventTypeId;
		std::function<void(const InputEvent&)> handler;

		template<class T>
		static InputEventHandler Create(const std::function<void(InputEvent)>& _handler) noexcept {
			return InputEventHandler(_handler, typeid(T).hash_code());
		}

	private:
		InputEventHandler(const std::function<void(InputEvent)>& _handler, size_t _eventTypeId) noexcept :
			handler(_handler),
			eventTypeId(_eventTypeId)
		{}
	};
}
