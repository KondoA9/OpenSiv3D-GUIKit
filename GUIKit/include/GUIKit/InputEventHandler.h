#pragma once

#include "InputEvent.h"

namespace s3d::gui {
	class InputEventHandler {
	public:
		const size_t eventTypeId;
		const std::function<void(const InputEvent&)> handler;

		template<class T>
		static InputEventHandler Create(const std::function<void(InputEvent)>& _handler) {
			return InputEventHandler(_handler, typeid(T).hash_code());
		}

		const InputEventHandler& operator =(const InputEventHandler& e) {
			assert(eventTypeId == e.eventTypeId);
			return *this;
		}

	private:
		InputEventHandler(const std::function<void(InputEvent)>& _handler, size_t _eventTypeId) :
			handler(_handler),
			eventTypeId(_eventTypeId)
		{}
	};
}
