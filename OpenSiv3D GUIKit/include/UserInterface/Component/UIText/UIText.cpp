#include "UIText.h"

using namespace s3d::gui;

void UIText::draw() {
	if (!drawable()) {
		return;
	}

	UIRect::draw();

	const double top = m_rect.y + paddingTop;
	const double bottom = m_rect.y + m_rect.h - paddingBottom;
	const double centerY = m_rect.y + m_rect.h * 0.5 + paddingTop - paddingBottom;
	const double left = m_rect.x + paddingLeft;
	const double right = m_rect.x + m_rect.w - paddingRight;
	const double centerX = m_rect.x + m_rect.w * 0.5 + paddingLeft - paddingRight;

	switch (direction)
	{
	case TextDirection::LeftTop:
		m_drawingRect = m_font(text).draw(Arg::topLeft(left, top), textColor);
		break;

	case TextDirection::LeftCenter:
		m_drawingRect = m_font(text).draw(Arg::leftCenter(left, centerY), textColor);
		break;

	case TextDirection::LeftBottom:
		m_drawingRect = m_font(text).draw(Arg::bottomLeft(left, bottom), textColor);
		break;

	case TextDirection::CenterTop:
		m_drawingRect = m_font(text).draw(Arg::topCenter(centerX, top), textColor);
		break;

	case TextDirection::Center:
		m_drawingRect = m_font(text).draw(Arg::center(centerX, centerY), textColor);
		break;

	case TextDirection::CenterBottom:
		m_drawingRect = m_font(text).draw(Arg::bottomCenter(centerX, bottom), textColor);
		break;

	case TextDirection::RightTop:
		m_drawingRect = m_font(text).draw(Arg::topRight(right, top), textColor);
		break;

	case TextDirection::RightCenter:
		m_drawingRect = m_font(text).draw(Arg::rightCenter(right, centerY), textColor);
		break;

	case TextDirection::RightBottom:
		m_drawingRect = m_font(text).draw(Arg::bottomRight(right, bottom), textColor);
		break;
	}
}

void UIText::setPadding(double top, double bottom, double left, double right) {
	paddingTop = top;
	paddingBottom = bottom;
	paddingLeft = left;
	paddingRight = right;
}