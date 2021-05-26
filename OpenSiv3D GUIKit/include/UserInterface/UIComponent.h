#pragma once

#include "Layer/Layer.h"
#include "../ColorTheme/DynamicColor.h"
#include "MouseEvent/MouseEvent.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIComponent {
	private:
		Array<Layer*> m_dependentLayer;
		Array<MouseEventHandler> m_mouseEventHandlers;
		bool m_needToUpdateLayer = true;

	protected:
		Layer m_layer;
		bool m_mouseOver = false, m_preMouseOver = false;

	public:
		ColorTheme backgroundColor;

	public:
		UIComponent(const ColorTheme& _backgroundColor = DynamicColor::BackgroundSecondary) :
			backgroundColor(_backgroundColor)
		{}

		virtual ~UIComponent() = default;

		virtual void updateLayer() {
			for (auto layer : m_dependentLayer) {
				layer->updateConstraints();
			}
			m_layer.updateConstraints();
		}

		virtual bool updateLayerIfNeeded() {
			if (m_needToUpdateLayer) {
				updateLayer();
				m_needToUpdateLayer = false;
				return true;
			}

			return false;
		}

		virtual void draw() {};

		virtual void updateMouseEvent() {
			mouseClicked();
			mouseHovered();
			mouseHovering();
			mouseUnHovered();
			mouseDragging();
			mouseWheel();
		}

		void addEventListener(MouseEventType e, const std::function<void(const MouseEvent& e)>& f) {
			m_mouseEventHandlers.push_back(MouseEventHandler(e, f));
		}

		void addEventListener(MouseEventType e, const std::function<void()>& f) {
			m_mouseEventHandlers.push_back(MouseEventHandler(e, [f](const MouseEvent&) {f(); }));
		}

		void setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant = 0.0, double multiplier = 1.0) {
			m_dependentLayer.push_back(&component.m_layer);
			auto myConstraint = m_layer.constraintPtr(direction);
			const auto opponentConstraint = component.m_layer.constraintPtr(toDirection);

			myConstraint->setConstraint(&opponentConstraint->value, constant, multiplier);
			m_needToUpdateLayer = true;
		}

		void setConstraint(LayerDirection direction, double constant = 0.0, double multiplier = 1.0) {
			auto myConstraint = m_layer.constraintPtr(direction);

			myConstraint->setConstraint(constant, multiplier);
			m_needToUpdateLayer = true;
		}

		void setConstraint(LayerDirection direction, const std::function<double()>& func, double constant = 0.0, double multiplier = 1.0) {
			auto myConstraint = m_layer.constraintPtr(direction);

			myConstraint->setConstraint(func, constant, multiplier);
			m_needToUpdateLayer = true;
		}

		void removeConstraint(LayerDirection direction) {
			auto constraint = m_layer.constraintPtr(direction);
			constraint->removeConstraint();
		}

		void requestToUpdateLayer() {
			m_needToUpdateLayer = true;
		}

	protected:
		void callMouseEventHandler(const MouseEvent& e) {
			for (auto& handler : m_mouseEventHandlers) {
				if (handler.eventType == e.type) {
					handler.handler(e);
				}
			}
		}

		// Run by loop
		virtual bool mouseClicked() = 0;
		virtual bool mouseHovered() = 0;
		virtual bool mouseHovering() = 0;
		virtual bool mouseUnHovered() = 0;
		virtual bool mouseDragging() = 0;
		virtual bool mouseWheel() = 0;
	};
}