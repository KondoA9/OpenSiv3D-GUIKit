#include <GUIKit/UIComponent.hpp>
#include <GUIKit/GUIFactory.hpp>

namespace s3d::gui {
	Array<UIComponent::CallableInputEvent> UIComponent::m_CallableInputEvents;
	std::shared_ptr<UIComponent> UIComponent::m_FocusedComponent = nullptr, UIComponent::m_PreviousFocusedComponent = nullptr;

	UIComponent::UIComponent(const ColorTheme& _backgroundColor, const ColorTheme& _frameColor) noexcept :
		m_id(GUIFactory::GetId()),
		backgroundColor(_backgroundColor),
		frameColor(_frameColor)
	{}

	UIComponent::~UIComponent() {
		GUIFactory::RequestReleaseComponent(m_id);
		release();
	}

	void UIComponent::initialize() {
		FMT_ASSERT(m_valid, "Make sure you instantiated through GUIFactory::Create()");
	}

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
		m_layer.constraintPtr(direction)->setConstraint(component.m_layer.constraintPtr(toDirection)->data(), constant, multiplier);
		m_needToUpdateLayer = true;
	}

	void UIComponent::setConstraint(LayerDirection direction, double constant, double multiplier) {
		m_layer.constraintPtr(direction)->setConstraint(constant, multiplier);
		m_needToUpdateLayer = true;
	}

	void UIComponent::setConstraint(LayerDirection direction, const std::function<double()>& func, double constant, double multiplier) {
		m_layer.constraintPtr(direction)->setConstraint(func, constant, multiplier);
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

	void UIComponent::focus() {
		try {
			// Focused component is this
			m_FocusedComponent = GUIFactory::GetComponent(m_id);;
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
