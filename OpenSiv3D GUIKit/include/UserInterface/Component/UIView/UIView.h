#pragma once

#include "../UIRect/UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIView : public UIRect {
	protected:
		Array<UIComponent*> m_userInterfaces;

	public:
		UIView(const ColorTheme& _backgroundColor = DynamicColor::Background) :
			UIRect(_backgroundColor)
		{}

		~UIView() {}

		virtual void appendComponent(UIComponent& ui) {
			m_userInterfaces.push_back(&ui);
		}

		void updateLayer() override;

		bool updateLayerIfNeeded() override;

		void draw() override;

		void updateMouseEvent() override;

		void updateLayerInvert();
	};
}