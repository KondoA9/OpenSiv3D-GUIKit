#include <GUIKit/UIText.h>

using namespace s3d::gui;

void UIText::updateLayer(const Rect& scissor) {
	UIRect::updateLayer(scissor);
	updateDrawableText();
}

void UIText::draw() {
	UIRect::draw();

	m_drawableText.draw(m_drawingRect, textColor);

	m_textRegion = m_drawingRect.size.asPoint();
}

void UIText::setPadding(double top, double bottom, double left, double right) {
	paddingTop = top;
	paddingBottom = bottom;
	paddingLeft = left;
	paddingRight = right;
	updateDrawableText();
}

void UIText::setFont(UnifiedFontStyle style) {
	m_font = UnifiedFont::Get(style);
	updateDrawableText();
}

void UIText::setText(const String& text) {
	m_text = text;
	updateDrawableText();
}

void UIText::setDirection(TextDirection direction) {
	m_direction = direction;
	updateDrawableText();
}

void UIText::updateDrawableText() {
	m_drawableText = m_font(m_text);

	const double top = m_rect.y + paddingTop;
	const double bottom = m_rect.y + m_rect.h - paddingBottom;
	const double centerY = m_rect.y + m_rect.h * 0.5 + paddingTop - paddingBottom;
	const double left = m_rect.x + paddingLeft;
	const double right = m_rect.x + m_rect.w - paddingRight;
	const double centerX = m_rect.x + m_rect.w * 0.5 + paddingLeft - paddingRight;

	switch (m_direction)
	{
	case TextDirection::LeftTop:
		m_drawingRect = m_drawableText.region(Arg::topLeft(left, top));
		break;

	case TextDirection::LeftCenter:
		m_drawingRect = m_drawableText.region(Arg::leftCenter(left, centerY));
		break;

	case TextDirection::LeftBottom:
		m_drawingRect = m_drawableText.region(Arg::bottomLeft(left, bottom));
		break;

	case TextDirection::CenterTop:
		m_drawingRect = m_drawableText.region(Arg::topCenter(centerX, top));
		break;

	case TextDirection::Center:
		m_drawingRect = m_drawableText.region(Arg::center(centerX, centerY));
		break;

	case TextDirection::CenterBottom:
		m_drawingRect = m_drawableText.region(Arg::bottomCenter(centerX, bottom));
		break;

	case TextDirection::RightTop:
		m_drawingRect = m_drawableText.region(Arg::topRight(right, top));
		break;

	case TextDirection::RightCenter:
		m_drawingRect = m_drawableText.region(Arg::rightCenter(right, centerY));
		break;

	case TextDirection::RightBottom:
		m_drawingRect = m_drawableText.region(Arg::bottomRight(right, bottom));
		break;
	}
}
