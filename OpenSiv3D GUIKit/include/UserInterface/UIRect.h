#pragma once

#include "UIComponent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIRect : public UIComponent {
	protected:
		Rect m_rect;

	public:
		using UIComponent::UIComponent;

		void updateLayer() override {
			UIComponent::updateLayer();
			m_rect = Rect(
				static_cast<int>(m_layer.left.value),
				static_cast<int>(m_layer.top.value),
				static_cast<int>(m_layer.width.value),
				static_cast<int>(m_layer.height.value)
			);
		}

		void draw() override {
			m_rect.draw(backgroundColor.color());
		}

	protected:
		bool mouseClicked() override {
			if (m_rect.leftClicked()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Clicked, this));
				return true;
			}
			return false;
		}

		bool mouseHovered() override {
			if (!m_preMouseOver && m_mouseOver) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovered, this));
				return true;
			}
			return false;
		}

		bool mouseHovering() override {
			if (m_mouseOver) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovering, this));
				return true;
			}
			return false;
		}

		bool mouseUnHovered() override {
			if (m_preMouseOver && !m_mouseOver) {
				callMouseEventHandler(MouseEvent(MouseEventType::UnHovered, this));
				return true;
			}
			return false;
		}

		bool mouseDragging() override {
			if (m_rect.leftPressed()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
				return true;
			}
			return false;
		}

		bool mouseWheel() override {
			if (const double wheel = Mouse::Wheel(); m_mouseOver && wheel != 0.0) {
				callMouseEventHandler(MouseEvent(MouseEventType::Wheel, this));
				return true;
			}
			return false;
		}

		void updateMouseEvent() override {
			m_preMouseOver = m_mouseOver;
			m_mouseOver = m_rect.mouseOver();
			UIComponent::updateMouseEvent();
		}
	};
}