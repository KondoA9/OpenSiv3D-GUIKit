#include "Aoba/UIText.hpp"

#include "Aoba/PixelUnit.hpp"

namespace s3d::aoba {
	void UIText::initialize() {
		UIRect::initialize();

		backgroundColor = DynamicColor::Clear;
	}

	void UIText::updateLayer(const Rect& scissor) {
		updateDrawableText();

		UIRect::updateLayer(scissor);

		updateDrawableText(true);
	}

	void UIText::draw() const {
		UIRect::draw();

		m_font(m_text).draw(m_textRegion, textColor);
	}

	void UIText::setPadding(double top, double bottom, double left, double right) {
		paddingTop = top;
		paddingBottom = bottom;
		paddingLeft = left;
		paddingRight = right;
		requestToUpdateLayer();
	}

	void UIText::setFont(UnifiedFontStyle style) {
		m_font = UnifiedFont::Get(style);
		requestToUpdateLayer();
	}

	void UIText::setText(const String& text) {
		m_text = text;
		requestToUpdateLayer();
	}

	void UIText::setDirection(TextDirection direction) {
		m_direction = direction;
		requestToUpdateLayer();
	}

	void UIText::updateDrawableText(bool updateField) {
		updateTextRegion();

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

	void UIText::updateTextRegion() {
		const double top = layer().top() + paddingTop;
		const double bottom = layer().bottom() - paddingBottom;
		const double centerY = layer().centerY() + paddingTop - paddingBottom;
		const double left = layer().left() + paddingLeft + 3.0_px;
		const double right = layer().right() - paddingRight - 3.0_px;
		const double centerX = layer().centerX() + paddingLeft - paddingRight;

		switch (m_direction)
		{
		case TextDirection::LeftTop:
			m_textRegion = m_font(m_text).region(Arg::topLeft(left, top));
			break;

		case TextDirection::LeftCenter:
			m_textRegion = m_font(m_text).region(Arg::leftCenter(left, centerY));
			break;

		case TextDirection::LeftBottom:
			m_textRegion = m_font(m_text).region(Arg::bottomLeft(left, bottom));
			break;

		case TextDirection::CenterTop:
			m_textRegion = m_font(m_text).region(Arg::topCenter(centerX, top));
			break;

		case TextDirection::Center:
			m_textRegion = m_font(m_text).region(Arg::center(centerX, centerY));
			break;

		case TextDirection::CenterBottom:
			m_textRegion = m_font(m_text).region(Arg::bottomCenter(centerX, bottom));
			break;

		case TextDirection::RightTop:
			m_textRegion = m_font(m_text).region(Arg::topRight(right, top));
			break;

		case TextDirection::RightCenter:
			m_textRegion = m_font(m_text).region(Arg::rightCenter(right, centerY));
			break;

		case TextDirection::RightBottom:
			m_textRegion = m_font(m_text).region(Arg::bottomRight(right, bottom));
			break;
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
