#include "Aoba/UIView.hpp"

#include "src/ComponentStorage/ComponentStorage.hpp"
#include "src/InputEvent/InputEventManager.hpp"

namespace s3d::aoba {
    void UIView::initialize() {
        UIRect::initialize();

        backgroundColor     = DynamicColor::Clear;
        penetrateMouseEvent = true;
    }

    void UIView::release() {
        for (auto const component : m_components) {
            component->_destroy();
        }

        m_components.release();

        UIRect::release();
    }

    void UIView::appendComponent(const UIComponent& component) {
        // The component cannot be appended multiple times
        assert(!m_components.includes_if(
            [&component](UIComponent* const component2) { return component.id() == component2->id(); }));

        m_components.emplace_back(&ComponentStorage::Get(component.id()));
    }

    void UIView::update() {
        UIRect::update();

        for (auto const component : m_components) {
            if (component->updatable()) {
                component->update();
            }
        }
    }

    void UIView::updateLayer(const Rect& scissor) {
        updateScissorRect(scissor);

        UIRect::updateLayer(scissor);

        for (auto const component : m_components) {
            if (component->layerUpdatable()) {
                component->updateLayer(m_scissorRect);
            }
        }
    }

    void UIView::updateLayerInvert(const Rect& scissor) {
        updateScissorRect(scissor);

        for (int i = static_cast<int>(m_components.size()) - 1; i >= 0; i--) {
            if (m_components[i]->exist) {
                m_components[i]->updateLayer(m_scissorRect);
            }
        }

        UIRect::updateLayer(scissor);
    }

    bool UIView::updateLayerIfNeeded(const Rect& scissor) {
        if (UIRect::updateLayerIfNeeded(scissor)) {
            updateScissorRect(scissor);
            for (auto const component : m_components) {
                if (component->exist) {
                    component->updateLayer(m_scissorRect);
                }
            }
            return true;
        } else {
            bool updated = false;
            for (auto const component : m_components) {
                if (component->exist) {
                    updated |= component->updateLayerIfNeeded(m_scissorRect);
                }
            }
            return updated;
        }
    }

    void UIView::draw() const {
        Graphics2D::SetScissorRect(m_parentScissorRect);

        UIRect::draw();

        Graphics2D::SetScissorRect(m_scissorRect);

        for (auto const component : m_components) {
            if (component->drawable()) {
                component->draw();
            }
        }

        Graphics2D::SetScissorRect(m_parentScissorRect);
    }

    void UIView::_destroy() {
        for (auto const component : m_components) {
            component->_destroy();
        }

        m_components.release();

        UIRect::_destroy();
    }

    void UIView::updateMouseIntersection() {
        UIRect::updateMouseIntersection();

        for (auto const component : m_components) {
            if (component->eventUpdatable()) {
                component->updateMouseIntersection();
            }
        }
    }

    void UIView::updateInputEvents() {
        UIRect::updateInputEvents();

        for (auto const component : m_components) {
            if (component->eventUpdatable()) {
                component->updateInputEvents();
            }
        }
    }

    void UIView::updateScissorRect(const Rect& parentScissorRect) noexcept {
        const auto left     = Max(parentScissorRect.x, static_cast<int>(layer().left()));
        const auto top      = Max(parentScissorRect.y, static_cast<int>(layer().top()));
        const auto right    = Min(parentScissorRect.x + parentScissorRect.w, static_cast<int>(layer().right()));
        const auto bottom   = Min(parentScissorRect.y + parentScissorRect.h, static_cast<int>(layer().bottom()));
        m_parentScissorRect = parentScissorRect;
        m_scissorRect       = Rect(left, top, right - left, bottom - top);
    }
}
