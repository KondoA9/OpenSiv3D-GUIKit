#pragma once

#include <Siv3D.hpp>

#include "Aoba/Factory.hpp"
#include "Aoba/UIText.hpp"

namespace s3d::aoba {
    class Tooltip {
    private:
        Optional<size_t> m_timeoutId = none;

        UIText& m_uiTooltipText = Factory::Create<UIText>();

    public:
        static void SetHoveredComponent(size_t id);

        static void Hide();

    private:
        Tooltip();

        static Tooltip& Instance();
    };
}
