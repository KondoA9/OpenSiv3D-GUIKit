#pragma once

#include "../UIComponent.h"

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
		void update() override;

		void draw() override;

		void updateLayer() override;

		bool mouseLeftDown() override;

		bool mouseLeftUp() override;

		bool mouseLeftDragging() override;

		bool mouseRightDown() override;

		bool mouseRightUp() override;

		bool mouseRightDragging() override;

		bool mouseHovered() override;

		bool mouseHovering() override;

		bool mouseUnHovered() override;

		bool mouseWheel() override;
	};
}