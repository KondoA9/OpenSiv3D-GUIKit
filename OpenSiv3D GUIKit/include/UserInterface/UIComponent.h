#pragma once

#include "Layer/Layer.h"
#include "../ColorTheme/DynamicColor.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	enum class MouseEvent: size_t {
		Clicked,
		Hovered,
		Hovering,
		UnHovered,
		Dragging
	};
	
	class UIComponent {
	private:
		// Event handler
		std::vector <std::function<void(UIComponent& component)>> m_mouseEventHandlers;
	protected:
		/*std::function<void(UIComponent& component)>& mouseEventHandler(MouseEvent e) {
			const size_t index = static_cast<size_t>(e);
			if (index < m_mouseEventHandlers.size()) {
				return m_mouseEventHandlers[index];
			}
			throw "Error";
		}*/
		void callMouseEventHandler(UIComponent& component, MouseEvent e) {
			const size_t index = static_cast<size_t>(e);
			if (index < m_mouseEventHandlers.size() && m_mouseEventHandlers[index]) {
				m_mouseEventHandlers[index](component);
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
			backgroundColor(_backgroundColor),
			m_mouseEventHandlers(std::vector <std::function<void(UIComponent& component)>>(5))
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

		void addEventListener(MouseEvent e, std::function<void(UIComponent& component)> f) {
			m_mouseEventHandlers[static_cast<size_t>(e)] = f;
		}

		void addEventListener(MouseEvent e, const std::function<void()>& f) {
			m_mouseEventHandlers[static_cast<size_t>(e)] = [f](UIComponent&) {f(); };
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