#pragma once

#include "Aoba/UIComponent.hpp"

namespace s3d::aoba {
    class ComponentStorage {
    private:
        Array<std::shared_ptr<UIComponent>> m_components;

        size_t m_releaseCounter = 0;

    public:
        ComponentStorage(const ComponentStorage&) = delete;

        ComponentStorage(ComponentStorage&&) = delete;

        ComponentStorage& operator=(const ComponentStorage&) = delete;

        ComponentStorage& operator=(ComponentStorage&&) = delete;

        static const std::shared_ptr<UIComponent>& Get(size_t id);

        static bool Has(size_t id);

        static void Store(const std::shared_ptr<UIComponent>& component);

        static void Release(size_t id);

    private:
        ComponentStorage() = default;

        ~ComponentStorage() = default;

        static ComponentStorage& Instance();

        void releaseComponentsIfNeed();

        void releaseUnusedComponents();
    };
}
