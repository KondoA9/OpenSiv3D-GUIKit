#pragma once

#include "../UIRect/UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;

	class UIView : public UIRect {
		friend GUIKit;

	protected:
		Array<UIComponent*> m_userInterfaces;

	public:
		UIView(const ColorTheme& _backgroundColor = DynamicColor::Background) :
			UIRect(_backgroundColor)
		{}

		virtual ~UIView() {}

		virtual void appendComponent(UIComponent& ui) {
			m_userInterfaces.push_back(&ui);
		}

		void updateLayerInvert();

	protected:
		void updateLayer() override;

		bool updateLayerIfNeeded() override;

		void draw() override;

		void updateMouseEvent() override;
	};
}