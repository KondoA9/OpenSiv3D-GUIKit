#pragma once

#include <Siv3D.hpp>

#include "Core.hpp"
#include "UIComponent.hpp"
#include "UIView.hpp"

namespace s3d::aoba {
	class Page;

	class Factory {
		friend Core;
		friend UIComponent;
		friend UIView;
		friend Page;

	private:
		static size_t m_Id, m_PreviousId;
		static Factory m_Instance;

		size_t m_releaseCounter = 0;
		Array<std::shared_ptr<UIComponent>> m_components;

	public:
		Factory(const Factory&) = delete;

		Factory(Factory&&) = delete;

		Factory& operator =(const Factory&) = delete;

		Factory& operator =(Factory&&) = delete;

		template<class T>
		[[nodiscard]] static T& Create(UIView& parent) {
			auto& component = Factory::CreateComponent<T>();

			parent.appendComponent(component);
			parent.onAfterComponentAppended();

			return component;
		}

		template<class T>
		[[nodiscard]] static T& Create(UIView* parent) {
			return Factory::Create<T>(*parent);
		}

		// Create component without parent component
		template<class T>
		[[nodiscard]] static T& Create() {
			auto& component = Factory::CreateComponent<T>();
			Core::AppendIsolatedComponent(component);
			return component;
		}

	private:
		Factory() = default;

		static size_t GetId();

		static std::shared_ptr<UIComponent>& GetComponent(size_t id);

		static void RequestReleaseComponent(size_t id);

		static void ReleaseInvalidComponents();

		static void ReleaseComponentsIfNeed();

		template<class T>
		[[nodiscard]] static T& CreateComponent() {
			ReleaseComponentsIfNeed();

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
