#pragma once

#include "UIComponent.h"

namespace s3d::gui {
	class UICircle : public UIComponent {
	public:
		Circle circle;

	public:
		using UIComponent::UIComponent;

		void updateShape() override {
			UIComponent::updateShape();
			circle = Circle(Arg::center(static_cast<int>(layer.centerX.value), static_cast<int>(layer.centerY.value)), (layer.height.value + layer.width.value) * 0.25);
		}

		void draw() override {
			UIComponent::draw();
			circle.draw(backgroundColor);
		}

	private:
		bool clicked() override {
			if (circle.leftClicked()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Clicked, this));
				return true;
			}
			return false;
		}

		bool hovered() override {
			static bool hovered = false;
			if (hovered && !circle.mouseOver()) {
				hovered = false;
			}
			else if (circle.mouseOver()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovered, this));
				hovered = true;
				return true;
			}
			return false;
		}

		bool hovering() override {
			if (circle.mouseOver()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Hovering, this));
				return true;
			}
			return false;
		}

		bool unHovered() override {
			static bool hovered = false;
			if (hovered && !circle.mouseOver()) {
				if (circle.mouseOver()) {
					callMouseEventHandler(MouseEvent(MouseEventType::UnHovered, this));
					return true;
				}
				hovered = false;
			}
			else if (circle.mouseOver()) {
				hovered = true;
			}
			return false;
		}

		bool dragging() override {
			if (circle.leftPressed()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
				return true;
			}
			return false;
		}
	};
}