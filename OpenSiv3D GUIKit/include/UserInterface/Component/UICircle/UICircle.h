#pragma once

#include "../UIComponent.h"

namespace s3d::gui {
	class UICircle : public UIComponent {
	protected:
		Circle m_circle;

	public:
		using UIComponent::UIComponent;

		void updateLayer() override;

		void draw() override;

	private:
		bool mouseClicked() override;

		bool mouseUp() override;

		bool mouseHovered() override;

		bool mouseHovering() override;

		bool mouseUnHovered() override;

		bool mouseDragging() override;

		bool mouseWheel() override;

		void updateMouseEvent() override;
	};
}