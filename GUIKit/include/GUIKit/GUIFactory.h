#pragma once

#include "UIComponent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKitCore;
	class UIView;

	class GUIFactory {
		friend GUIKitCore;
		friend UIComponent;
		friend UIView;

	private:
		static size_t m_Id, m_PreviousId;
		static GUIFactory instance;

		size_t m_releaseCounter = 0;
		Array<std::shared_ptr<UIComponent>> m_components;

	public:
		GUIFactory(const GUIFactory&) = delete;

		GUIFactory(GUIFactory&&) = delete;

		GUIFactory& operator =(const GUIFactory&) = delete;

		GUIFactory& operator =(GUIFactory&&) = delete;

		template<class T>
		static T& Create() {
			if (instance.m_releaseCounter++; instance.m_releaseCounter == 100) {
				ReleaseInvalidComponents();

				if (instance.m_components.capacity() != instance.m_components.size()) {
					instance.m_components.shrink_to_fit();
				}

				instance.m_releaseCounter = 0;
			}

			m_Id++;

			auto component = std::shared_ptr<T>(new T());
			component->validate();

			instance.m_components.push_back(component);

			return *static_cast<T*>(component.get());
		}

	private:
		GUIFactory() = default;

		static size_t GetId();

		static std::shared_ptr<UIComponent>& GetComponent(size_t id);

		static void RequestReleaseComponent(size_t id);

		static void ReleaseInvalidComponents();
	};
}
