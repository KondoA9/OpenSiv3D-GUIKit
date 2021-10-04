#pragma once

#include "UIRect.hpp"

#include <Siv3D.hpp>

namespace s3d::gui {
	class PageManager;

	class UIView : public UIRect {
		friend PageManager;
		friend GUIFactory;

	private:
		Array<std::shared_ptr<UIComponent>> m_components;
		Rect m_scissorRect = Rect(0, 0, 0, 0), m_parentScissorRect = Rect(0, 0, 0, 0);

	public:
		explicit UIView() noexcept :
			UIRect(DynamicColor::Background)
		{}

		virtual ~UIView() {}

		void release() override;

		size_t componentsCount() const {
			return m_components.size();
		}

		const UIComponent& getComponent(size_t index) const {
			return *m_components[index].get();
		}

		template<class T>
		const T& getComponent(size_t index) const {
			return *static_cast<T*>(m_components[index].get());
		}

		UIComponent& getComponent(size_t index) {
			return *m_components[index].get();
		}

		template<class T>
		T& getComponent(size_t index) {
			return *static_cast<T*>(m_components[index].get());
		}

	protected:
		// This function runs after a component appended. gui::GUIFactory::Create<UIComponent>(this);
		virtual void onAfterComponentAppended() {}

		void updateLayer(const Rect& scissor) override;

		bool updateLayerIfNeeded(const Rect& scissor) override;

		void draw() override;

		void updateInputEvents() override;

	private:
		void appendComponent(const UIComponent& component);

		void updateMouseIntersection() final;

		void updateLayerInvert(const Rect& scissor);

		void updateScissorRect(const Rect& parentScissorRect);
	};
}
