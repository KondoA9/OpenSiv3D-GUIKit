#include "UIComponent.h"

using namespace s3d::gui;

void UIComponent::updateLayer() {
	for (auto layer : m_dependentLayer) {
		layer->updateConstraints();
	}
	m_layer.updateConstraints();
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
	mouseClicked();
	mouseHovered();
	mouseHovering();
	mouseUnHovered();
	mouseDragging();
	mouseWheel();
}

void UIComponent::setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant, double multiplier) {
	m_dependentLayer.push_back(&component.m_layer);
	auto myConstraint = m_layer.constraintPtr(direction);
	const auto opponentConstraint = component.m_layer.constraintPtr(toDirection);

	myConstraint->setConstraint(&opponentConstraint->value, constant, multiplier);
	m_needToUpdateLayer = true;
}

void UIComponent::setConstraint(LayerDirection direction, double constant, double multiplier) {
	auto myConstraint = m_layer.constraintPtr(direction);

	myConstraint->setConstraint(constant, multiplier);
	m_needToUpdateLayer = true;
}

void UIComponent::setConstraint(LayerDirection direction, const std::function<double()>& func, double constant, double multiplier) {
	auto myConstraint = m_layer.constraintPtr(direction);

	myConstraint->setConstraint(func, constant, multiplier);
	m_needToUpdateLayer = true;
}

void UIComponent::removeConstraint(LayerDirection direction) {
	auto constraint = m_layer.constraintPtr(direction);
	constraint->removeConstraint();
}

void UIComponent::callMouseEventHandler(const MouseEvent& e) const {
	for (auto& handler : m_mouseEventHandlers) {
		if (handler.eventType == e.type) {
			handler.handler(e);
		}
	}
}