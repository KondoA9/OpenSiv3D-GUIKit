#pragma once

#include "UIComponent.hpp"
#include "UIView.hpp"
#include "AobaCore.hpp"

#include <Siv3D.hpp>

namespace s3d::aoba {
	class Page;

	class AobaFactory {
		friend AobaCore;
		friend UIComponent;
		friend UIView;
		friend Page;

	private:
		static size_t m_Id, m_PreviousId;
		static AobaFactory m_Instance;

		size_t m_releaseCounter = 0;
		Array<std::shared_ptr<UIComponent>> m_components;

	public:
		AobaFactory(const AobaFactory&) = delete;

		AobaFactory(AobaFactory&&) = delete;

		AobaFactory& operator =(const AobaFactory&) = delete;

		AobaFactory& operator =(AobaFactory&&) = delete;

		template<class T>
		[[nodiscard]] static T& Create(UIView& parent) {
			auto& component = AobaFactory::CreateComponent<T>();

			parent.appendComponent(component);
			parent.onAfterComponentAppended();

			return component;
		}

		template<class T>
		[[nodiscard]] static T& Create(UIView* parent) {
			return AobaFactory::Create<T>(*parent);
		}

		// Create component without parent component
		template<class T>
		[[nodiscard]] static T& Create() {
			auto& component = AobaFactory::CreateComponent<T>();
			AobaCore::Instance().appendIsolatedComponent(component);
			return component;
		}

	private:
		AobaFactory() = default;

		static size_t GetId();

		static std::shared_ptr<UIComponent>& GetComponent(size_t id);

		static void RequestReleaseComponent(size_t id);

		static void ReleaseInvalidComponents();

		template<class T>
		[[nodiscard]] static T& CreateComponent() {
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
	};
}
