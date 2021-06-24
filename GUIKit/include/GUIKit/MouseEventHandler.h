#pragma once

#include "IMouseEvent.h"

namespace s3d::gui {
	class MouseEventHandler {
	public:
		size_t eventTypeId;
		std::function<void(const IMouseEvent&)> handler;

		MouseEventHandler(const std::function<void(IMouseEvent)>& _handler) :
			handler(_handler)
		{}

		template<class T>
		void setEvent() {
			eventTypeId = typeid(T).hash_code();
		}
	};
}