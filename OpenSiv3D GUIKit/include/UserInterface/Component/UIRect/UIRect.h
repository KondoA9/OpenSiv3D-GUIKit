#pragma once

#include "../UIComponent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIRect : public UIComponent {
	protected:
		Rect m_rect;

	public:
		using UIComponent::UIComponent;

		void draw() override;

		void updateLayer() override;

	protected:
		bool mouseDown() override;

		bool mouseUp() override;

		bool mouseHovered() override;

		bool mouseHovering() override;

		bool mouseUnHovered() override;

		bool mouseDragging() override;

		bool mouseWheel() override;

		void updateMouseEvent() override;
	};
}