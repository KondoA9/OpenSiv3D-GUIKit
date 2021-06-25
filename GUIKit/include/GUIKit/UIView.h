#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;

	class UIView : public UIRect {
		friend GUIKit;

	protected:
		Array<UIComponent*> m_userInterfaces;

	public:
		UIView(const ColorTheme& _backgroundColor = DynamicColor::Background) :
			UIRect(_backgroundColor),
			m_userInterfaces(Array<UIComponent*>(0))
		{}

		virtual ~UIView() {}

		virtual void appendComponent(UIComponent& ui);

	protected:
		void updateLayer() override;

		bool updateLayerIfNeeded() override;

		void draw() override;

	private:
		void updateMouseIntersection() override;

		void updateMouseEvents() override;

		void updateLayerInvert();
	};
}