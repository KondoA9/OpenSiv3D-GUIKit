#pragma once

#include "UIComponent.h"

namespace s3d::gui {
	class UICircle : public UIComponent {
	private:
		Circle m_circle;

	public:
		using UIComponent::UIComponent;

		const Circle& circle() const {
			return m_circle;
		}

	protected:
		void updateMouseIntersection() final;

		void updateLayer(const Rect& scissor) override;

		void draw() override;
	};
}
