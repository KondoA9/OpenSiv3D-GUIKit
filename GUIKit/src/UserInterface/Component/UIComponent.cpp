#include <GUIKit/UIComponent.h>

using namespace s3d::gui;

Array<UIComponent::CallableMouseEvent> UIComponent::m_CallableMouseEvents;
UIComponent* UIComponent::m_FocusedComponent = nullptr;

void UIComponent::updateLayer() {
	if (!exist) {
		return;
	}

	for (auto layer : m_dependentLayers) {
		layer->updateConstraints();
	}
	m_layer.updateConstraints();
}

bool UIComponent::updateLayerIfNeeded() {
	if (!exist) {
		return false;
	}

	if (m_needToUpdateLayer) {
		updateLayer();
		m_needToUpdateLayer = false;
		return true;
	}

	return false;
}

void UIComponent::updateMouseEvent() {
	if (!exist || !controllable) {
		return;
	}

	bool result = false;

	if (!result) result |= mouseLeftDown();
	if (!result) result |= mouseLeftUp();
	if (!result) result |= mouseLeftDragging();
	if (!result) result |= mouseRightDown();
	if (!result) result |= mouseRightUp();
	if (!result) result |= mouseRightDragging();
	if (!result) result |= mouseHovered();
	if (!result) result |= mouseHovering();
	if (!result) result |= mouseUnHovered();
	if (!result) result |= mouseWheel();
}

void UIComponent::setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant, double multiplier) {
	m_dependentLayers.push_back(&component.m_layer);

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

void UIComponent::callMouseEventHandler(const MouseEvent& e) const {
	const auto hendlers = m_mouseEventHandlers.removed_if([e](const MouseEventHandler& handler) {
		return handler.eventType != e.type;
		});

	if (m_CallableMouseEvents.size() == 0 || e.component->penetrateMouseEvent) {
		m_CallableMouseEvents.push_back({ e, hendlers });
	}
	else {
		for (size_t i : step(m_CallableMouseEvents.size())) {
			if (m_CallableMouseEvents[i].mouseEvent.type == e.type) {
				m_CallableMouseEvents[i].mouseEvent = e;
				m_CallableMouseEvents[i].handlers = hendlers;
				break;
			}
			else if (i == m_CallableMouseEvents.size() - 1) {
				m_CallableMouseEvents.push_back({ e, hendlers });
			}
		}
	}
}

void UIComponent::ResetMouseEvents() {
	m_CallableMouseEvents.release();
}

void UIComponent::CallMouseEvents() {
	for (const auto& e : m_CallableMouseEvents) {
		for (const auto& handler : e.handlers) {
			handler.handler(e.mouseEvent);
		}
	}
}