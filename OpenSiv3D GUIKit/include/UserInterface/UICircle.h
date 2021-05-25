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
		void clicked() override {
			if (onClicked && circle.leftClicked()) {
				onClicked(*this);
			}
		}

		void hovered() override {
			if (onHovered && circle.mouseOver()) {
				onHovered(*this);
			}
		}
	};
}