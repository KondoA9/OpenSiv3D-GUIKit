#include "src/Tooltip/Tooltip.hpp"

#include "Aoba/Core.hpp"
#include "Aoba/PixelUnit.hpp"
#include "src/ComponentStorage/ComponentStorage.hpp"

namespace s3d::aoba {
    Tooltip::Tooltip() {
        m_uiTooltipText.hidden          = true;
        m_uiTooltipText.drawFrame       = true;
        m_uiTooltipText.frameThickness  = 1.0_px;
        m_uiTooltipText.frameColor      = DynamicColor::Separator;
        m_uiTooltipText.backgroundColor = DynamicColor::Background;
        m_uiTooltipText.tooltipDisabled = true;
        m_uiTooltipText.penetrateMouseEvent = true;
        m_uiTooltipText.setCornerRadius(2_px);
        m_uiTooltipText.setDirection(TextDirection::Center);
        m_uiTooltipText.setConstraint(LayerDirection::Top, [this] {
            const auto y = Cursor::Pos().y + 18_px;
            const auto h = m_uiTooltipText.textRegion().h + 8_px;
            if (y + h > Scene::Size().y) {
                return y - 18_px - static_cast<int32>(h);
            } else {
                return y;
            }
        });
        m_uiTooltipText.setConstraint(LayerDirection::Left, [this] {
            const auto x = Cursor::Pos().x;
            const auto w = m_uiTooltipText.textRegion().w + 16_px;
            if (x + w > Scene::Size().x) {
                return x - static_cast<int32>(w);
            } else {
                return x;
            }
        });
        m_uiTooltipText.setConstraint(LayerDirection::Height, [this] { return m_uiTooltipText.textRegion().h + 8_px; });
        m_uiTooltipText.setConstraint(LayerDirection::Width, [this] { return m_uiTooltipText.textRegion().w + 16_px; });
    }

    Tooltip& Tooltip::Instance() {
        static Tooltip instance;
        return instance;
    }

    void Tooltip::SetHoveredComponent(size_t id) {
        auto& component = ComponentStorage::Get(id);

        if (component->tooltipMessage.isEmpty()) {
            return;
        }

        if (Instance().m_timeoutId) {
            Core::StopTimeout(Instance().m_timeoutId.value());
        }

        auto& instance         = Instance();
        Instance().m_timeoutId = Core::SetTimeout(
            [&instance, &component] {
                instance.m_uiTooltipText.hidden = false;
                instance.m_uiTooltipText.setText(component->tooltipMessage);
            },
            500,
            false);
    }

    void Tooltip::Hide() {
        Instance().m_uiTooltipText.hidden = true;
    }
}
