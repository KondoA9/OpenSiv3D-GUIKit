#pragma once

#include "UIComponent.h"

namespace s3d::gui {
	class UICircle : public UIComponent {
	protected:
		Circle m_circle;

	public:
		using UIComponent::UIComponent;

		void updateLayer() override {
			UIComponent::updateLayer();
			m_circle = Circle(Arg::center(static_cast<int>(m_layer.centerX.value), static_cast<int>(m_layer.centerY.value)), (m_layer.height.value + m_layer.width.value) * 0.25);
		}

		void draw() override {
			UIComponent::draw();
			m_circle.draw(backgroundColor);
		}

	private:
		bool clicked() override {
			if (m_circle.leftClicked()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Clicked, this));
				return true;
			}
			return false;
		}

		bool hovered() override {
			static bool hovered = false;
			if (hovered && !m_mouseOver) {
				hovered = false;
			}
			else if (m_mouseOver) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovered, this));
				hovered = true;
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
			static bool hovered = false;
			if (hovered && !m_mouseOver) {
				if (m_mouseOver) {
					callMouseEventHandler(MouseEvent(MouseEventType::UnHovered, this));
					return true;
				}
				hovered = false;
			}
			else if (m_mouseOver) {
				hovered = true;
			}
			return false;
		}

		bool dragging() override {
			if (m_circle.leftPressed()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
				return true;
			}
			return false;
		}

		void updateMouseEvent() override {
			m_mouseOver = m_circle.mouseOver();
			UIComponent::updateMouseEvent();
		}
	};
}