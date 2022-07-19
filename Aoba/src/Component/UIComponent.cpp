#include "Aoba/UIComponent.hpp"

#include "src/ComponentStorage/ComponentStorage.hpp"
#include "src/InputEvent/InputEventManager.hpp"
#include "src/KeyShortcut/KeyShortcut.hpp"
#include "src/Tooltip/Tooltip.hpp"

namespace s3d::aoba {
    Optional<size_t> UIComponent::m_FocusedComponentId = none, UIComponent::m_PreviousFocusedComponentId = none;

    UIComponent::UIComponent(size_t id) :
        backgroundColor(DynamicColor::BackgroundSecondary), frameColor(DynamicColor::Separator), m_id(id) {}

    UIComponent::~UIComponent() {}

    void UIComponent::release() {
        removeAllConstraints();
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

    void UIComponent::update() {
        m_constraintsUpdatedThisFrame = false;

        if (isFocused()) {
            for (auto& shortcut : m_keyShortcuts) {
                if (shortcut->keyDown()) {
                    shortcut->call();
                }
            }
        }
    }

    void UIComponent::updateLayer(const Rect& scissor) {
        if (!m_initializedColors) {
            initializeColors();
            m_initializedColors = true;
        }

        m_drawableRegion = scissor;

        updateConstraints();
    }

    bool UIComponent::updateLayerIfNeeded(const Rect& scissor) {
        if (m_needToUpdateLayer) {
            updateLayer(scissor);
            m_needToUpdateLayer = false;
            return true;
        }

        return false;
    }

    void UIComponent::updateConstraints() {
        if (!m_constraintsUpdatedThisFrame) {
            // Update layer of dependent components before updating self
            for (auto const component : m_dependentComponents) {
                component->updateConstraints();
            }

            m_layer.updateConstraints();
        }
        m_constraintsUpdatedThisFrame = true;
    }

    void UIComponent::setConstraint(LayerDirection direction,
                                    UIComponent& component,
                                    LayerDirection toDirection,
                                    double constant,
                                    double multiplier) {
        if (!m_dependentComponents.includes_if(
                [&component](UIComponent* const dependent) { return dependent->id() == component.id(); })) {
            m_dependentComponents.emplace_back(&ComponentStorage::Get(component.id()));
        }

        m_layer.setConstraint(direction, component.m_layer, toDirection, constant, multiplier);
        m_needToUpdateLayer = true;
    }

    void UIComponent::setConstraint(LayerDirection direction,
                                    UIComponent* component,
                                    LayerDirection toDirection,
                                    double constant,
                                    double multiplier) {
        setConstraint(direction, *component, toDirection, constant, multiplier);
    }

    void UIComponent::setConstraint(LayerDirection direction,
                                    const std::function<double()>& func,
                                    double constant,
                                    double multiplier) {
        m_layer.setConstraint(direction, func, constant, multiplier);
        m_needToUpdateLayer = true;
    }

    void UIComponent::setConstraint(LayerDirection direction, double constant) noexcept {
        m_layer.setConstraint(direction, constant);
        m_needToUpdateLayer = true;
    }

    void UIComponent::removeConstraint(LayerDirection direction) noexcept {
        m_layer.removeConstraint(direction);
    }

    void UIComponent::removeAllConstraints() noexcept {
        m_layer.removeAllConstraints();
        m_dependentComponents.release();
    }

    void UIComponent::focus() noexcept {
        if (ComponentStorage::Has(m_id)) {
            // Focused component is this
            m_FocusedComponentId = m_id;
        } else {
            m_FocusedComponentId = none;
        }
    }

    void UIComponent::unFocus() noexcept {
        if (isFocused()) {
            m_FocusedComponentId = none;
        }
    }

    void UIComponent::registerKeyShortcut(const Input& input, const std::function<void()>& callback) {
        m_keyShortcuts.emplace_back(std::make_unique<KeyShortcut<Input>>(input, callback));
    }

    void UIComponent::registerKeyShortcut(const InputCombination& input, const std::function<void()>& callback) {
        m_keyShortcuts.emplace_back(std::make_unique<KeyShortcut<InputCombination>>(input, callback));
    }

    void UIComponent::registerKeyShortcut(const InputGroup& input, const std::function<void()>& callback) {
        m_keyShortcuts.emplace_back(std::make_unique<KeyShortcut<InputGroup>>(input, callback));
    }
}
