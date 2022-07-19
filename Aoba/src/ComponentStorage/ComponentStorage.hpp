#pragma once

#include "Aoba/UIComponent.hpp"

namespace s3d::aoba {
    class ComponentStorage {
    private:
        Array<std::shared_ptr<UIComponent>> m_components;
        Array<std::shared_ptr<UIComponent>> m_isolatedComponents;

    public:
        ComponentStorage(const ComponentStorage&) = delete;

        ComponentStorage(ComponentStorage&&) = delete;

        ComponentStorage& operator=(const ComponentStorage&) = delete;

        ComponentStorage& operator=(ComponentStorage&&) = delete;

        static const std::shared_ptr<UIComponent>& Get(size_t id);

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

        static const std::shared_ptr<UIComponent>& Store(std::shared_ptr<UIComponent>&& component);

        static const std::shared_ptr<UIComponent>& StoreIsolated(std::shared_ptr<UIComponent>&& component);

        static void Release(size_t id);

    private:
        ComponentStorage() = default;

        ~ComponentStorage() = default;

        static ComponentStorage& Instance();
    };
}
