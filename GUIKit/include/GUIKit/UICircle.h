#pragma once

#include "UIComponent.h"

namespace s3d::gui {
	class UICircle : public UIComponent {
	protected:
		Circle m_circle;

	public:
		using UIComponent::UIComponent;

		Circle circle() const {
			return m_circle;
		}

	protected:
		void updateMouseIntersection() final;

		void draw(const Rect&) override;

		void updateLayer() override;
	};
}
