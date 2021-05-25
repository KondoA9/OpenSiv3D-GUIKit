#pragma once

#include "Layer/Layer.h"
#include "../ColorTheme/DynamicColor.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	enum class MouseEvent {
		Clicked,
		Hovered,
		Dragged
	};

	class UIComponent {
	public:
		ColorTheme backgroundColor;

	protected:
		Layer layer;
		bool shouldUpdateLayer = true;
		std::function<void(UIComponent& component)> onClicked;
		std::function<void(UIComponent& component)> onHovered;
		std::function<void(UIComponent& component)> onDragged;

	public:
		UIComponent(const ColorTheme& _backgroundColor = DynamicColor::backgroundSecondary) :
			backgroundColor(_backgroundColor)
		{}

		virtual void updateShape() {
			layer.updateConstraints();
		}

		virtual bool updateShapeIfNeeded() {
			if (shouldUpdateLayer) {
				updateShape();
				shouldUpdateLayer = false;
				return true;
			}

			return false;
		}

		virtual void draw() {
			control();
		}

		void addEventListener(MouseEvent e, std::function<void(UIComponent& component)> f) {
			switch (e)
			{
			case MouseEvent::Clicked:
				onClicked = f;
				break;
			case MouseEvent::Hovered:
				onHovered = f;
				break;
			case MouseEvent::Dragged:
				onDragged = f;
				break;
			}
		}

		void addEventListener(MouseEvent e, const std::function<void()>& f) {
			switch (e)
			{
			case MouseEvent::Clicked:
				onClicked = [f](UIComponent&) {f(); };
				break;
			case MouseEvent::Hovered:
				onHovered = [f](UIComponent&) {f(); };
				break;
			case MouseEvent::Dragged:
				onDragged = [f](UIComponent&) {f(); };
				break;
			}
		}

		void setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant = 0.0, double multiplier = 1.0) {
			auto myConstraint = layer.constraintPtr(direction);
			const auto opponentConstraint = component.layer.constraintPtr(toDirection);

			myConstraint->setConstraint(&opponentConstraint->value, constant, multiplier);
			shouldUpdateLayer = true;
		}

		void setConstraint(LayerDirection direction, double constant = 0.0, double multiplier = 1.0) {
			auto myConstraint = layer.constraintPtr(direction);

			myConstraint->setConstraint(constant, multiplier);
			shouldUpdateLayer = true;
		}

		void setConstraint(LayerDirection direction, const std::function<double()>& func, double constant = 0.0, double multiplier = 1.0) {
			auto myConstraint = layer.constraintPtr(direction);

			myConstraint->setConstraint(func, constant, multiplier);
			shouldUpdateLayer = true;
		}

		void removeConstraint(LayerDirection direction) {
			auto constraint = layer.constraintPtr(direction);
			constraint->removeConstraint();
		}

	protected:
		virtual void clicked() {};
		virtual void hovered() {};
		virtual void dragged() {};

	private:
		void control() {
			clicked();
			hovered();
			dragged();
		}
	};
}