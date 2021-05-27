#include "UIComponent.h"

using namespace s3d::gui;

Array<UIComponent::CallableMouseEvent> UIComponent::m_callableMouseEvents;

void UIComponent::updateLayer() {
	for (auto l : m_dependentLayer) {
		l->updateConstraints();
	}
	layer.updateConstraints();
}

bool UIComponent::updateLayerIfNeeded() {
	if (m_needToUpdateLayer) {
		updateLayer();
		m_needToUpdateLayer = false;
		return true;
	}

	return false;
}

void UIComponent::updateMouseEvent() {
	mouseDown();
	mouseUp();
	mouseHovered();
	mouseHovering();
	mouseUnHovered();
	mouseDragging();
	mouseWheel();
}

void UIComponent::setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant, double multiplier) {
	m_dependentLayer.push_back(&component.layer);
	auto myConstraint = layer.constraintPtr(direction);
	const auto opponentConstraint = component.layer.constraintPtr(toDirection);

	myConstraint->setConstraint(&opponentConstraint->value, constant, multiplier);
	m_needToUpdateLayer = true;
}

void UIComponent::setConstraint(LayerDirection direction, double constant, double multiplier) {
	auto myConstraint = layer.constraintPtr(direction);

	myConstraint->setConstraint(constant, multiplier);
	m_needToUpdateLayer = true;
}

void UIComponent::setConstraint(LayerDirection direction, const std::function<double()>& func, double constant, double multiplier) {
	auto myConstraint = layer.constraintPtr(direction);

	myConstraint->setConstraint(func, constant, multiplier);
	m_needToUpdateLayer = true;
}

void UIComponent::removeConstraint(LayerDirection direction) {
	auto constraint = layer.constraintPtr(direction);
	constraint->removeConstraint();
}

void UIComponent::callMouseEventHandler(const MouseEvent& e) const {
	const auto hendlers = m_mouseEventHandlers.removed_if([e](const MouseEventHandler& handler) {
		return handler.eventType != e.type;
		});

	if (m_callableMouseEvents.size() == 0 || e.component->penetrateMouseEvent) {
		m_callableMouseEvents.push_back({ e, hendlers });
	}
	else {
		for (size_t i : step(m_callableMouseEvents.size())) {
			if (m_callableMouseEvents[i].mouseEvent.type == e.type) {
				m_callableMouseEvents[i].mouseEvent = e;
				m_callableMouseEvents[i].handlers = hendlers;
				break;
			}
			else if (i == m_callableMouseEvents.size() - 1) {
				m_callableMouseEvents.push_back({ e, hendlers });
			}
		}
	}
}

void UIComponent::ResetMouseEvents() {
	m_callableMouseEvents.release();
}

void UIComponent::CallMouseEvents() {
	for (const auto& e : m_callableMouseEvents) {
		for (const auto& handler : e.handlers) {
			handler.handler(e.mouseEvent);
		}
	}
}