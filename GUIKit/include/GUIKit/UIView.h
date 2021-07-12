#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;

	class UIView : public UIRect {
		friend GUIKit;

	protected:
		Array<UIComponent*> m_userInterfaces;

	private:
		Array<UIComponent*> m_deletableComponents;
		Rect m_scissorRect = Rect(0, 0, 0, 0), m_parentScissorRect = Rect(0, 0, 0, 0);

	public:
		UIView(const ColorTheme& _backgroundColor = DynamicColor::Background) :
			UIRect(_backgroundColor),
			m_userInterfaces(Array<UIComponent*>(0))
		{}

		virtual ~UIView() {
			releaseDeletableComponents();
		}

		virtual void appendComponent(UIComponent& ui);

		template<class T>
		T& appendTemporaryComponent(const T& component) {
			T* cmp = new T(component);
			UIView::appendComponent(*cmp);
			m_deletableComponents.push_back(cmp);
			return *cmp;
		}

	protected:
		void releaseDeletableComponents();

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
