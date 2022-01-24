#include <Aoba/UIComponent.hpp>

#include "src/ComponentStorage/ComponentStorage.hpp"

namespace s3d::aoba {
	Array<UIComponent::CallableInputEvent> UIComponent::m_CallableInputEvents;
	std::shared_ptr<UIComponent> UIComponent::m_FocusedComponent = nullptr, UIComponent::m_PreviousFocusedComponent = nullptr;

	UIComponent::UIComponent(size_t id) noexcept :
		backgroundColor(DynamicColor::BackgroundSecondary),
		frameColor(DynamicColor::Separator),
		m_id(id)
	{}

	UIComponent::~UIComponent() {
		_destroy();
	}

	void UIComponent::_destroy() {
		release();
		ComponentStorage::Release(m_id);
	}

	void UIComponent::updateLayer(const Rect& scissor) {
		m_drawableRegion = scissor;

		if (!m_initialized) {
			//initialize();
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
		m_layer.setConstraint(direction, component.m_layer, toDirection, constant, multiplier);
		m_needToUpdateLayer = true;
	}

	void UIComponent::setConstraint(LayerDirection direction, double constant, double multiplier) {
		m_layer.setConstraint(direction, constant, multiplier);
		m_needToUpdateLayer = true;
	}

	void UIComponent::setConstraint(LayerDirection direction, const std::function<double()>& func, double constant, double multiplier) {
		m_layer.setConstraint(direction, func, constant, multiplier);
		m_needToUpdateLayer = true;
	}

	void UIComponent::removeConstraint(LayerDirection direction) {
		m_layer.removeConstraint(direction);
	}

	void UIComponent::removeAllConstraints() {
		m_layer.removeAllConstraints();
	}

	void UIComponent::focus() {
		try {
			// Focused component is this
			m_FocusedComponent = ComponentStorage::Get(m_id);;
		}
		catch (...) {
			m_FocusedComponent.reset();
		}
	}

	void UIComponent::unFocus() {
		if (isFocused()) {
			m_FocusedComponent.reset();
		}
	}
}
