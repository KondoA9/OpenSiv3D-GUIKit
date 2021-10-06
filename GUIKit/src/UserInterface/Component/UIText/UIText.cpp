#include <GUIKit/UIText.hpp>
#include <GUIKit/PixelUnit.hpp>

namespace s3d::gui {
	void UIText::updateLayer(const Rect& scissor) {
		updateDrawableText();

		UIRect::updateLayer(scissor);

		updateDrawableText(true);
	}

	void UIText::draw() {
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

			updateDrawableRegion();

			m_textRegion.x -= 2_px;
			m_textRegion.y -= 2_px;
			m_textRegion.w += 4_px;
			m_textRegion.h += 4_px;
		}
	}

	void UIText::updateTextRegion() {
		const double top = rect().y + paddingTop;
		const double bottom = rect().y + rect().h - paddingBottom;
		const double centerY = rect().y + rect().h * 0.5 + paddingTop - paddingBottom;
		const double left = rect().x + paddingLeft + 3.0_px;
		const double right = rect().x + rect().w - paddingRight - 3.0_px;
		const double centerX = rect().x + rect().w * 0.5 + paddingLeft - paddingRight;

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
		if (m_textRegion.x < rect().x) {
			m_textRegion.x = rect().x;
		}

		if (m_textRegion.y < rect().y) {
			m_textRegion.y = rect().y;
		}

		// Word wrap
		if (const auto lines = static_cast<int>(oneLineWidth / rect().w) + 1; lines != 1) {
			m_textRegion.w = rect().w;
			m_textRegion.h *= lines;
		}

		// Fit overhang region
		if (const auto bottom = rect().y + rect().h; m_textRegion.y + m_textRegion.h > bottom) {
			m_textRegion.h = bottom - m_textRegion.y;
		}
	}

	void UIText::updateDrawableRegion() {
		const auto right = rect().x + rect().w - (m_textRegion.x - rect().x);
		const auto w = right - m_textRegion.x;

		const auto h = rect().h - (m_textRegion.y - rect().y);

		m_drawableRegion = RectF(m_textRegion.x - 3_px, m_textRegion.y - 2_px, w + 6_px, h + 4_px);
	}
}
