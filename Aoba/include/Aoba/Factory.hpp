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

		template<class T>
		[[nodiscard]] static T& CreateComponent() {
			const size_t id = m_Id++;

#if SIV3D_BUILD(DEBUG)
			Logger << U"[Aoba](Create) " + Unicode::Widen(std::string(typeid(T).name())) + U" " + ToString(id);
#endif

			return *static_cast<T*>(m_Instance.storeComponent(std::shared_ptr<T>(new T())).get());
		}

		std::shared_ptr<UIComponent>& storeComponent(const std::shared_ptr<UIComponent>& component);
	};
}
