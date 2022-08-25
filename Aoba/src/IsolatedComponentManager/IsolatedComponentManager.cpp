#include "IsolatedComponentManager.hpp"

namespace s3d::aoba {
    IsolatedComponentManager& IsolatedComponentManager::Instance() {
        static IsolatedComponentManager instance;
        return instance;
    }

    void IsolatedComponentManager::Store(UIComponent& component) {
        Instance().m_components.emplace_back(component);
    }

    void IsolatedComponentManager::Update() {
        for (const auto& component : Instance().m_components) {
            component.get().update();
        }
    }

    void IsolatedComponentManager::UpdateLayer(const Rect& scissorRect) {
        for (const auto& component : Instance().m_components) {
            component.get().updateLayer(scissorRect);
        }
    }

    void IsolatedComponentManager::UpdateLayerIfNeed(const Rect& scissorRect) {
        for (const auto& component : Instance().m_components) {
            component.get().updateLayerIfNeeded(scissorRect);
        }
    }

    void IsolatedComponentManager::UpdateEvent() {
        for (const auto& component : Instance().m_components) {
            if (component.get().isOperatable()) {
                component.get().updateMouseIntersection();
                component.get().updateInputEvents();
            }
        }
    }

    void IsolatedComponentManager::Draw() {
        for (const auto& component : Instance().m_components) {
            if (component.get().isDrawable()) {
                component.get().draw();
            }
        }
    }
}
