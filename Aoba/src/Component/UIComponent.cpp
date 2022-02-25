#include "Aoba/UIComponent.hpp"

#include "src/ComponentStorage/ComponentStorage.hpp"
#include "src/InputEvent/InputEventManager.hpp"
#include "src/Tooltip/Tooltip.hpp"

namespace s3d::aoba {
    Optional<size_t> UIComponent::m_FocusedComponentId = none, UIComponent::m_PreviousFocusedComponentId = none;

    UIComponent::UIComponent(size_t id) noexcept :
        backgroundColor(DynamicColor::BackgroundSecondary), frameColor(DynamicColor::Separator), m_id(id) {}

    UIComponent::~UIComponent() {
        _destroy();
    }

    void UIComponent::_destroy() {
        release();
        InputEventManager::Unregister(m_id);
        ComponentStorage::Release(m_id);
    }

    void UIComponent::initialize() {
        addEventListener<Event::Mouse::Hovered>([this] {
            if (!tooltipDisabled) {
                Tooltip::SetHoveredComponent(m_id);
            }
        });
        addEventListener<Event::Mouse::UnHovered>([this] {
            if (!tooltipDisabled) {
                Tooltip::Hide(m_id);
            }
        });
        addEventListener<Event::Mouse::LeftDownRaw>([this] {
            if (!tooltipDisabled) {
                Tooltip::Hide(m_id);
            }
        });
        addEventListener<Event::Mouse::RightDownRaw>([this] {
            if (!tooltipDisabled) {
                Tooltip::Hide(m_id);
            }
        });
    }

    void UIComponent::updateLayer(const Rect& scissor) {
        if (!m_initializedColors) {
            initializeColors();
            m_initializedColors = true;
        }

        m_drawableRegion = scissor;

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

    void UIComponent::setConstraint(LayerDirection direction,
                                    UIComponent& component,
                                    LayerDirection toDirection,
                                    double constant,
                                    double multiplier) {
        m_dependentLayers.emplace_back(&component.m_layer);
        m_layer.setConstraint(direction, component.m_layer, toDirection, constant, multiplier);
        m_needToUpdateLayer = true;
    }

    void UIComponent::setConstraint(LayerDirection direction, double constant, double multiplier) {
        m_layer.setConstraint(direction, constant, multiplier);
        m_needToUpdateLayer = true;
    }

    void UIComponent::setConstraint(LayerDirection direction,
                                    const std::function<double()>& func,
                                    double constant,
                                    double multiplier) {
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
        if (ComponentStorage::Has(m_id)) {
            // Focused component is this
            m_FocusedComponentId = m_id;
        } else {
            m_FocusedComponentId = none;
        }
    }

    void UIComponent::unFocus() {
        if (isFocused()) {
            m_FocusedComponentId = none;
        }
    }
}
