#include <GUIKit/UIComponent.h>

using namespace s3d::gui;

Array<UIComponent::CallableInputEvent> UIComponent::m_CallableInputEvents;
UIComponent* UIComponent::m_FocusedComponent = nullptr;

void UIComponent::updateLayer(const Rect& scissor) {
	m_drawableRegion = scissor;

	if (!m_initialized) {
		initialize();
		m_initialized = true;
	}

	for (auto layer : m_dependentLayers) {
		layer->updateConstraints();
	}
	m_layer.updateConstraints();
}

bool UIComponent::updateLayerIfNeeded(const Rect& scissor) {
	if (m_needToUpdateLayer) {
		updateLayer(scissor);
		m_needToUpdateLayer = false;
		return true;
	}

	return false;
}

void UIComponent::setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant, double multiplier) {
	m_dependentLayers.push_back(&component.m_layer);

	auto myConstraint = m_layer.constraintPtr(direction);
	const auto opponentConstraint = component.m_layer.constraintPtr(toDirection);

	myConstraint->setConstraint(opponentConstraint->data(), constant, multiplier);
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

void UIComponent::removeAllConstraints() {
	m_layer.top.removeConstraint();
	m_layer.bottom.removeConstraint();
	m_layer.left.removeConstraint();
	m_layer.right.removeConstraint();
	m_layer.centerX.removeConstraint();
	m_layer.centerY.removeConstraint();
	m_layer.height.removeConstraint();
	m_layer.width.removeConstraint();
}
