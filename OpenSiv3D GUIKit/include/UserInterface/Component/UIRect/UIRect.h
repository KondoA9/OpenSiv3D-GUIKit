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

		void updateMouseEvent() override;
	};
}