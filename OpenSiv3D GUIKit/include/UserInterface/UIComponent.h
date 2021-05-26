#pragma once

#include "Layer/Layer.h"
#include "../ColorTheme/DynamicColor.h"
#include "MouseEvent/MouseEvent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIComponent {
	private:
		std::vector <MouseEventHandler> m_mouseEventHandlers;

	protected:
		void callMouseEventHandler(const MouseEvent& e) {
			for (auto& handler : m_mouseEventHandlers) {
				if (handler.eventType == e.type) {
					handler.handler(e);
				}
			}
		}

		// Run by loop
		virtual bool clicked() = 0;
		virtual bool hovered() = 0;
		virtual bool hovering() = 0;
		virtual bool unHovered() = 0;
		virtual bool dragging() = 0;

	public:
		ColorTheme backgroundColor;

	protected:
		Layer layer;
		bool shouldUpdateLayer = true;

	public:
		UIComponent(const ColorTheme& _backgroundColor = DynamicColor::backgroundSecondary) :
			backgroundColor(_backgroundColor)
		{}

		virtual ~UIComponent() = default;

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

		virtual void draw() {};

		virtual void control() {
			clicked();
			hovered();
			hovering();
			unHovered();
			dragging();
		}

		void addEventListener(MouseEventType e, const std::function<void(const MouseEvent& e)>& f) {
			m_mouseEventHandlers.push_back(MouseEventHandler(e, f));
		}

		void addEventListener(MouseEventType e, const std::function<void()>& f) {
			m_mouseEventHandlers.push_back(MouseEventHandler(e, [f](const MouseEvent&) {f(); }));
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
	};
}