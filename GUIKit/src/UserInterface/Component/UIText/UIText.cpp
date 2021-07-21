#include <GUIKit/UIText.h>
#include <GUIKit/PixelUnit.h>

using namespace s3d::gui;

void UIText::updateLayer(const Rect& scissor) {
	UIRect::updateLayer(scissor);

	updateDrawableText(true);
}

void UIText::draw() {
	UIRect::draw();

	m_drawableText.draw(m_drawableRegion, textColor);
}

const UIText& UIText::setPadding(double top, double bottom, double left, double right) {
	paddingTop = top;
	paddingBottom = bottom;
	paddingLeft = left;
	paddingRight = right;
	updateDrawableText();

	return *this;
}

const UIText& UIText::setFont(UnifiedFontStyle style) {
	m_font = UnifiedFont::Get(style);
	updateDrawableText(true);

	return *this;
}

const UIText& UIText::setText(const String& text) {
	m_text = text;
	updateDrawableText(true);

	return *this;
}

const UIText& UIText::setDirection(TextDirection direction) {
	m_direction = direction;
	updateDrawableText();

	return *this;
}

void UIText::updateDrawableText(bool updateField) {
	m_drawableText = m_font(m_text);

	updateTextRegion();

	if (updateField) {
		fitTextRegionToRect();

		updateDrawableRegion();
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

void::UIText::fitTextRegionToRect() {
	if (m_textRegion.w > rect().w) {
		m_textRegion.h *= static_cast<int>(m_textRegion.w / rect().w) + 1;
	}

	if (m_textRegion.x < rect().x) {
		m_textRegion.x = rect().x;
	}

	if (m_textRegion.y < rect().y) {
		m_textRegion.y = rect().y;
	}

	if (const auto right = rect().x + rect().w; m_textRegion.x + m_textRegion.w > right) {
		m_textRegion.w = right - m_textRegion.x;
	}

	if (const auto bottom = rect().y + rect().h; m_textRegion.y + m_textRegion.h > bottom) {
		m_textRegion.h = bottom - m_textRegion.y;
	}
}

void UIText::updateDrawableRegion() {
	const auto right = rect().x + rect().w - (m_textRegion.x - rect().x);
	const auto w = right - m_textRegion.x;

	const auto h = rect().h - (m_textRegion.y - rect().y);

	m_drawableRegion = RectF(m_textRegion.x, m_textRegion.y, w, h + 4_px);
}
