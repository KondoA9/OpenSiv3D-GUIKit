#include "Aoba/UIText.hpp"

#include "Aoba/PixelUnit.hpp"

namespace s3d::aoba {
    void UIText::initialize() {
        UIRect::initialize();

        backgroundColor = DynamicColor::Clear;
    }

    void UIText::updateLayer(const Rect& scissor) {
        updateDrawableText(scissor);

        UIRect::updateLayer(scissor);

        updateDrawableText(scissor, true);
    }

    void UIText::draw() const {
        UIRect::draw();

        m_drawableText.draw(m_textRegion, textColor);
    }

    void UIText::setPadding(double top, double bottom, double left, double right) {
        paddingTop    = top;
        paddingBottom = bottom;
        paddingLeft   = left;
        paddingRight  = right;
        requestToUpdateLayer();
    }

    void UIText::setFont(const Font& font) {
        m_font         = font;
        m_drawableText = m_font(m_text);
        requestToUpdateLayer();
    }

    void UIText::setText(const String& text) {
        m_text         = text;
        m_drawableText = m_font(m_text);
        requestToUpdateLayer();
    }

    void UIText::setDirection(TextDirection direction) {
        m_direction = direction;
        requestToUpdateLayer();
    }

    void UIText::updateDrawableText(const Rect& scissor, bool updateField) {
        updateTextRegion(scissor);

        if (updateField) {
            fitTextRegionToRect();

            // Set margin
            {
                m_textRegion.h += 4_px;
                m_textRegion.w += m_font.fontSize();

                if (const auto right = m_textRegion.x + m_textRegion.w; right > layer().right()) {
                    m_textRegion.w = layer().right() - m_textRegion.x;
                }
            }
        }
    }

    void UIText::calcTextRegion() {
        const double top     = layer().top() + paddingTop;
        const double bottom  = layer().bottom() - paddingBottom;
        const double centerY = layer().centerY() + paddingTop - paddingBottom;
        const double left    = layer().left() + paddingLeft + 3.0_px;
        const double right   = layer().right() - paddingRight - 3.0_px;
        const double centerX = layer().centerX() + paddingLeft - paddingRight;

        switch (m_direction) {
        case TextDirection::LeftTop:
            m_textRegion = m_drawableText.region(Arg::topLeft(left, top));
            break;

        case TextDirection::LeftCenter:
            m_textRegion = m_drawableText.region(Arg::leftCenter(left, centerY));
            break;

        case TextDirection::LeftBottom:
            m_textRegion = m_drawableText.region(Arg::bottomLeft(left, bottom));
            break;

        case TextDirection::CenterTop:
            m_textRegion = m_drawableText.region(Arg::topCenter(centerX, top));
            break;

        case TextDirection::Center:
            m_textRegion = m_drawableText.region(Arg::center(centerX, centerY));
            break;

        case TextDirection::CenterBottom:
            m_textRegion = m_drawableText.region(Arg::bottomCenter(centerX, bottom));
            break;

        case TextDirection::RightTop:
            m_textRegion = m_drawableText.region(Arg::topRight(right, top));
            break;

        case TextDirection::RightCenter:
            m_textRegion = m_drawableText.region(Arg::rightCenter(right, centerY));
            break;

        case TextDirection::RightBottom:
            m_textRegion = m_drawableText.region(Arg::bottomRight(right, bottom));
            break;
        }
    }

    void UIText::updateTextRegion(const Rect& scissor) {
        if (scissor.intersects(layer().asRect())) {
            calcTextRegion();
        }
    }

    void UIText::fitTextRegionToRect() {
        const auto oneLineWidth = m_textRegion.w;

        // Fit starting pos
        if (m_textRegion.x < layer().left()) {
            m_textRegion.x = layer().left();
        }

        if (m_textRegion.y < layer().top()) {
            m_textRegion.y = layer().top();
        }

        // Word wrap
        if (const auto lines = static_cast<int>(oneLineWidth / layer().width()) + 1; lines != 1) {
            m_textRegion.w = layer().width();
            m_textRegion.h *= lines;
        }

        // Fit overhang region
        if (const auto bottom = layer().bottom(); m_textRegion.y + m_textRegion.h > bottom) {
            m_textRegion.h = bottom - m_textRegion.y;
        }
    }
}
