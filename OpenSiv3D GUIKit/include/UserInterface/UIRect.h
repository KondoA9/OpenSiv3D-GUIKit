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
		void clicked() override {
			if (onClicked && rect.leftClicked()) {
				onClicked(*this);
			}
		}

		void hovered() override {
			if (onHovered && rect.mouseOver()) {
				onHovered(*this);
			}
		}

		void dragged() override {
			if (onDragged && rect.leftPressed()) {
				onDragged(*this);
			}
		}
	};
}