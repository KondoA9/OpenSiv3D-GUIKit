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
			UIComponent::draw();
			rect.draw(backgroundColor.color());
		}

	protected:
		bool clicked() override {
			if (rect.leftClicked()) {
				callMouseEventHandler(*this, MouseEvent::Clicked);
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
				callMouseEventHandler(*this, MouseEvent::Hovered);
				h1 = true;
				return true;
			}
			return false;
		}

		bool hovering() override {
			if (rect.mouseOver()) {
				callMouseEventHandler(*this, MouseEvent::Hovering);
				return true;
			}
			return false;
		}

		bool h2 = false;
		bool unHovered() override {
			if (h2 && !rect.mouseOver()) {
				callMouseEventHandler(*this, MouseEvent::UnHovered);
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
				callMouseEventHandler(*this, MouseEvent::Dragging);
				return true;
			}
			return false;
		}
	};
}