#pragma once

#include "../UnifiedFont.hpp"
#include "UIRect.hpp"

namespace s3d::aoba {
    enum class TextDirection {
        LeftTop,
        LeftCenter,
        LeftBottom,
        CenterTop,
        Center,
        CenterBottom,
        RightTop,
        RightCenter,
        RightBottom
    };

    class UIText : public UIRect {
    public:
        ColorTheme textColor = DynamicColor::Text;

    private:
        String m_text             = U"";
        Font m_font               = UnifiedFont::Get();
        TextDirection m_direction = TextDirection::LeftCenter;
        RectF m_textRegion        = RectF();
        double paddingTop = 0.0, paddingBottom = 0.0, paddingLeft = 0.0, paddingRight = 0.0;

    public:
        using UIRect::UIRect;

        virtual ~UIText() {}

        const Font& font() const {
            return m_font;
        }

        const String& text() const {
            return m_text;
        }
        const RectF& textRegion() const {
            return m_textRegion;
        }

        virtual void setPadding(double top, double bottom, double left, double right);

        virtual void setFont(UnifiedFontStyle style);

        virtual void setText(const String& text);

        virtual void setDirection(TextDirection direction);

        virtual void setFont(const Font& font) {
            m_font = font;
        }

    protected:
        void initialize() override;

        void updateLayer(const Rect& scissor) override;

        void draw() const override;

        virtual void updateDrawableText(bool updateField = false);

    private:
        void updateTextRegion();

        void fitTextRegionToRect();
    };
}
