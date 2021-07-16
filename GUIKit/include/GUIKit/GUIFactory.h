#pragma once

#include "UIComponent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIFactory {
	private:
		static size_t m_ComponentId;
		Array<std::shared_ptr<UIComponent>> m_components;

	public:
		static GUIFactory& Instance() {
			static GUIFactory instance;
			return instance;
		}

		template<class T>
		static T& Create(const T& component) {
			Instance().m_components.push_back(std::make_shared<T>(component));
			return *std::dynamic_pointer_cast<T>(Instance().m_components[Instance().m_components.size() - 1]);
		}

	private:

	};
}
