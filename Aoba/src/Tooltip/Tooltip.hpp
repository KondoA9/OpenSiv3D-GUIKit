#pragma once

#include <Siv3D.hpp>

#include "Aoba/Factory.hpp"
#include "Aoba/UIText.hpp"

namespace s3d::aoba {
    class Tooltip {
    private:
        Optional<size_t> m_componentId = none;
        Optional<size_t> m_timeoutId = none;

        UIText& m_uiTooltipText = Factory::Create<UIText>();

    public:
        static void SetHoveredComponent(size_t componentId);

        static void Hide(size_t componentId);

    private:
        Tooltip();

        static Tooltip& Instance();
    };
}
