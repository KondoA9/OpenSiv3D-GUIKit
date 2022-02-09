#pragma once

#include "Aoba/Component/UIBase.hpp"

namespace s3d::aoba {
    class ComponentStorage {
    private:
        Array<std::shared_ptr<UIBase>> m_components;

        size_t m_releaseCounter = 0;

    public:
        ComponentStorage(const ComponentStorage&) = delete;

        ComponentStorage(ComponentStorage&&) = delete;

        ComponentStorage& operator=(const ComponentStorage&) = delete;

        ComponentStorage& operator=(ComponentStorage&&) = delete;

        static void Store(const std::shared_ptr<UIBase>& component);

        static void Release(size_t id);

        static std::shared_ptr<UIBase>& Get(size_t id);

    private:
        ComponentStorage() = default;

        ~ComponentStorage() = default;

        static ComponentStorage& Instance();

        void releaseComponentsIfNeed();

        void releaseUnusedComponents();
    };
}
