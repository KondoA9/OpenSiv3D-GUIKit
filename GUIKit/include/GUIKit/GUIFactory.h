#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIComponent;
	class UIView;

	class GUIFactory {
		friend UIComponent;
		friend UIView;

	private:
		static size_t m_Id;
		static GUIFactory instance;

		size_t m_releaseCounter = 0;
		Array<std::shared_ptr<UIComponent>> m_components;

	public:
		GUIFactory(const GUIFactory&) = delete;

		GUIFactory(GUIFactory&&) = delete;

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
			instance.m_components.push_back(std::shared_ptr<T>(new T()));
			return *static_cast<T*>(instance.m_components[instance.m_components.size() - 1].get());
		}

		GUIFactory& operator =(const GUIFactory&) = delete;

		GUIFactory& operator =(GUIFactory&&) = delete;

	private:
		GUIFactory() = default;

		static size_t GetId();

		static std::shared_ptr<UIComponent>& GetComponent(size_t id);

		static void RequestReleaseComponent(size_t id);

		static void ReleaseInvalidComponents();
	};
}
