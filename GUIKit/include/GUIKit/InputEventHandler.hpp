#pragma once

#include "InputEvent.hpp"

namespace s3d::gui {
	class InputEventHandler {
	public:
		size_t eventTypeId;
		std::function<void(const InputEvent&)> handler;

		template<class T>
		static InputEventHandler Create(const std::function<void(InputEvent)>& _handler) noexcept {
			return InputEventHandler(typeid(T).hash_code(), _handler);
		}

	private:
		InputEventHandler(size_t _eventTypeId, const std::function<void(InputEvent)>& _handler) noexcept :
			eventTypeId(_eventTypeId),
			handler(_handler)
		{}
	};
}
