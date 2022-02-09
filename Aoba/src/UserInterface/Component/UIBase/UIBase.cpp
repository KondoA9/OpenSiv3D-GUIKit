#include "Aoba/Component/UIBase.hpp"

#include "src/ComponentStorage/ComponentStorage.hpp"

namespace s3d::aoba {
    Array<UIBase::CallableInputEvent> UIBase::m_CallableInputEvents;
    std::shared_ptr<UIBase> UIBase::m_FocusedComponent         = nullptr,
                                 UIBase::m_PreviousFocusedComponent = nullptr;

    UIBase::UIBase(size_t id) noexcept :
        backgroundColor(DynamicColor::BackgroundSecondary), frameColor(DynamicColor::Separator), m_id(id) {}

    UIBase::~UIBase() {
        _destroy();
    }

    void UIBase::_destroy() {
        release();
        ComponentStorage::Release(m_id);
    }

    void UIBase::updateLayer(const Rect& scissor) {
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

    bool UIBase::updateLayerIfNeeded(const Rect& scissor) {
        if (m_needToUpdateLayer) {
            updateLayer(scissor);
            m_needToUpdateLayer = false;
            return true;
        }

        return false;
    }

    void UIBase::setConstraint(LayerDirection direction,
                                    UIBase& component,
                                    LayerDirection toDirection,
                                    double constant,
                                    double multiplier) {
        m_dependentLayers.push_back(&component.m_layer);
        m_layer.setConstraint(direction, component.m_layer, toDirection, constant, multiplier);
        m_needToUpdateLayer = true;
    }

    void UIBase::setConstraint(LayerDirection direction, double constant, double multiplier) {
        m_layer.setConstraint(direction, constant, multiplier);
        m_needToUpdateLayer = true;
    }

    void UIBase::setConstraint(LayerDirection direction,
                                    const std::function<double()>& func,
                                    double constant,
                                    double multiplier) {
        m_layer.setConstraint(direction, func, constant, multiplier);
        m_needToUpdateLayer = true;
    }

    void UIBase::removeConstraint(LayerDirection direction) {
        m_layer.removeConstraint(direction);
    }

    void UIBase::removeAllConstraints() {
        m_layer.removeAllConstraints();
    }

    void UIBase::focus() {
        try {
            // Focused component is this
            m_FocusedComponent = ComponentStorage::Get(m_id);
            ;
        } catch (...) {
            m_FocusedComponent.reset();
        }
    }

    void UIBase::unFocus() {
        if (isFocused()) {
            m_FocusedComponent.reset();
        }
    }
}
