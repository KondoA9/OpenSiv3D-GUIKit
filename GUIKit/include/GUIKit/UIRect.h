#pragma once

#include "UIComponent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIRect : public UIComponent {
	protected:
		Rect m_rect;

	public:
		using UIComponent::UIComponent;

		Rect rect() const {
			return m_rect;
		}

	protected:
		void updateMouseIntersection() override;

		void draw() override;

		void updateLayer() override;
	};
}