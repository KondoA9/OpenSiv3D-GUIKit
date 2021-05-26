#pragma once

#include "UIComponent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIRect : public UIComponent {
	protected:
		Rect rect;

	public:
		using UIComponent::UIComponent;

		void updateShape() override {
			UIComponent::updateShape();
			rect = Rect(
				static_cast<int>(layer.left.value),
				static_cast<int>(layer.top.value),
				static_cast<int>(layer.width.value),
				static_cast<int>(layer.height.value)
			);
		}

		void draw() override {
			rect.draw(backgroundColor.color());
		}

	protected:
		bool clicked() override {
			if (rect.leftClicked()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Clicked, this));
				return true;
			}
			return false;
		}

		bool h1 = false;
		bool hovered() override {
			if (h1 && !rect.mouseOver()) {
				h1 = false;
			}
			else if (rect.mouseOver()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovered, this));
				h1 = true;
				return true;
			}
			return false;
		}

		bool hovering() override {
			if (rect.mouseOver()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovering, this));
				return true;
			}
			return false;
		}

		bool h2 = false;
		bool unHovered() override {
			if (h2 && !rect.mouseOver()) {
				callMouseEventHandler(MouseEvent(MouseEventType::UnHovered, this));
				h2 = false;
				return true;
			}
			else if (rect.mouseOver()) {
				h2 = true;
			}
			return false;
		}

		bool dragging() override {
			if (rect.leftPressed()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
				return true;
			}
			return false;
		}
	};
}