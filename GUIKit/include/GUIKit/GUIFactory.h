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
		static GUIFactory m_Instance;

		size_t m_releaseCounter = 0;
		Array<std::shared_ptr<UIComponent>> m_components;

	public:
		GUIFactory(const GUIFactory&) = delete;

		GUIFactory(GUIFactory&&) = delete;

		GUIFactory& operator =(const GUIFactory&) = delete;

		GUIFactory& operator =(GUIFactory&&) = delete;

		template<class T>
		static T& Create() {
			if (m_Instance.m_releaseCounter++; m_Instance.m_releaseCounter == 100) {
				ReleaseInvalidComponents();

				if (m_Instance.m_components.capacity() != m_Instance.m_components.size()) {
					m_Instance.m_components.shrink_to_fit();
				}

				m_Instance.m_releaseCounter = 0;
			}

			m_Id++;

			{
				auto component = std::shared_ptr<T>(new T());
				component->validate();

				m_Instance.m_components.push_back(component);
			}

			return *static_cast<T*>(m_Instance.m_components.back().get());
		}

	private:
		GUIFactory() = default;

		static size_t GetId();

		static std::shared_ptr<UIComponent>& GetComponent(size_t id);

		static void RequestReleaseComponent(size_t id);

		static void ReleaseInvalidComponents();
	};
}
