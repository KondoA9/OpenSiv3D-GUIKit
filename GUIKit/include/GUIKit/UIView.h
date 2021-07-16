#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;

	class UIView : public UIRect {
		friend GUIKit;

	private:
		Array<std::shared_ptr<UIComponent>> m_components;
		Rect m_scissorRect = Rect(0, 0, 0, 0), m_parentScissorRect = Rect(0, 0, 0, 0);

	public:
		UIView() noexcept :
			UIRect(DynamicColor::Background)
		{}

		virtual ~UIView() {
			release();
		};

		void release() override;

		virtual void appendComponent(const UIComponent& component);

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
		void updateLayer(const Rect& scissor) override;

		bool updateLayerIfNeeded(const Rect& scissor) override;

		void draw() override;

		void updateInputEvents() override;

	private:
		void updateMouseIntersection() final;

		void updateLayerInvert(const Rect& scissor);

		void updateScissorRect(const Rect& parentScissorRect);
	};
}
