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
			UIRect(_backgroundColor)
		{}

		virtual ~UIView() {}

		virtual void appendComponent(UIComponent& ui) {
			m_userInterfaces.push_back(&ui);
		}

	protected:
		void updateMouseEvent() final;

		void updateLayer() override;

		bool updateLayerIfNeeded() override;

		void draw() override;

	private:
		void updateLayerInvert();
	};
}