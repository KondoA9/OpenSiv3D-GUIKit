#pragma once

#include "UIComponent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIFactory {
		friend UIComponent;

	private:
		static GUIFactory instance;
		Array<std::shared_ptr<UIComponent>> m_components;

	public:
		static GUIFactory& Instance() {
			return instance;
		}

		template<class T>
		static T& Create() {
			Instance().m_components.push_back(std::shared_ptr<T>(new T()));
			return *std::dynamic_pointer_cast<T>(Instance().m_components[Instance().m_components.size() - 1]);
		}

	private:
		const Array<std::shared_ptr<UIComponent>>& components() const {
			return m_components;
		}
	};
}
