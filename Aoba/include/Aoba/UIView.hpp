#pragma once

#include "UIRect.hpp"

namespace s3d::aoba {
	class Factory;
	class PageManager;

	class UIView : public UIRect {
		friend Factory;
		friend PageManager;

	private:
		Array<std::shared_ptr<UIComponent>> m_components;
		Rect m_scissorRect = Rect(0, 0, 0, 0), m_parentScissorRect = Rect(0, 0, 0, 0);

	public:
		UIView() noexcept :
			UIRect()
		{
			backgroundColor = DynamicColor::Background;
		}

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
		// This function runs after a component appended. gui::Factory::Create<UIComponent>(this);
		virtual void onAfterComponentAppended() {}

		void update() override;

		void updateLayer(const Rect& scissor) override;

		bool updateLayerIfNeeded(const Rect& scissor) override;

		void updateInputEvents() override;

		void draw() const override;

		void _destroy() override;

	private:
		void appendComponent(const UIComponent& component);

		void updateMouseIntersection() final;

		void updateLayerInvert(const Rect& scissor);

		void updateScissorRect(const Rect& parentScissorRect);
	};
}
