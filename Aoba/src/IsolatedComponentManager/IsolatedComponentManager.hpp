#pragma once

#include "Aoba/UIComponent.hpp"

namespace s3d::aoba {
    class IsolatedComponentManager {
    private:
        Array<std::reference_wrapper<UIComponent>> m_components;

    public:
        static void Store(UIComponent& component);

        static void Update();

        static void UpdateLayer(const Rect& scissorRect);

        static void UpdateLayerIfNeed(const Rect& scissorRect);

        static void UpdateEvent();

        static void Draw();

    private:
        IsolatedComponentManager() = default;

        static IsolatedComponentManager& Instance();
    };
}
