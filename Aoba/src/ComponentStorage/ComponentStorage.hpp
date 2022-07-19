#pragma once

#include "Aoba/UIComponent.hpp"

namespace s3d::aoba {
    class ComponentStorage {
    private:
        Array<std::unique_ptr<UIComponent>> m_components;
        Array<std::unique_ptr<UIComponent>> m_isolatedComponents;

    public:
        ComponentStorage(const ComponentStorage&) = delete;

        ComponentStorage(ComponentStorage&&) = delete;

        ComponentStorage& operator=(const ComponentStorage&) = delete;

        ComponentStorage& operator=(ComponentStorage&&) = delete;

        static UIComponent& Get(size_t id);

        static void MapComponents(const std::function<void(UIComponent&)>& func) {
            for (const auto& component : Instance().m_components) {
                func(*component);
            }
        }

        static void MapIsolatedComponents(const std::function<void(UIComponent&)>& func) {
            for (const auto& component : Instance().m_isolatedComponents) {
                func(*component);
            }
        }

        static bool Has(size_t id);

        static UIComponent& Store(std::unique_ptr<UIComponent>&& component);

        static UIComponent& StoreIsolated(std::unique_ptr<UIComponent>&& component);

        static bool Release(size_t id);

    private:
        ComponentStorage() = default;

        ~ComponentStorage() = default;

        static ComponentStorage& Instance();

        Optional<size_t> findComponentById(size_t id, bool isolated);

        UIComponent& insertComponent(std::unique_ptr<UIComponent>&& component, bool isolated);

        bool releaseComponent(size_t id, bool isolated);
    };
}
