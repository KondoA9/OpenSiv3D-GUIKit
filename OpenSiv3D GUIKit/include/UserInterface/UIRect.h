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
		bool clicked() override {
			if (m_rect.leftClicked()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Clicked, this));
				return true;
			}
			return false;
		}

		bool hovered() override {
			if (!m_preMouseOver && m_mouseOver) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovered, this));
				return true;
			}
			return false;
		}

		bool hovering() override {
			if (m_mouseOver) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovering, this));
				return true;
			}
			return false;
		}

		bool unHovered() override {
			if (m_preMouseOver && !m_mouseOver) {
				callMouseEventHandler(MouseEvent(MouseEventType::UnHovered, this));
				return true;
			}
			return false;
		}

		bool dragging() override {
			if (m_rect.leftPressed()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
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