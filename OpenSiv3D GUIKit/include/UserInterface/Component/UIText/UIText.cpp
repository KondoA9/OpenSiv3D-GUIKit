#include "UIText.h"

using namespace s3d::gui;

void UIText::draw() {
	if (!drawable()) {
		return;
	}

	UIRect::draw();

	const Color color = DynamicColor::Text;
	const auto t = UnifiedFont::Get(style)(text);

	const double top = m_rect.y + paddingTop;
	const double bottom = m_rect.y + m_rect.h - paddingBottom;
	const double centerY = m_rect.y + m_rect.h * 0.5 + paddingTop - paddingBottom;
	const double left = m_rect.x + paddingLeft;
	const double right = m_rect.x + m_rect.w - paddingRight;
	const double centerX = m_rect.x + m_rect.w * 0.5 + paddingLeft - paddingRight;

	switch (direction)
	{
	case s3d::gui::TextDirection::LeftTop:
		t.draw(Arg::topLeft(left, top), color);
		break;

	case s3d::gui::TextDirection::LeftCenter:
		t.draw(Arg::leftCenter(left, centerY), color);
		break;

	case s3d::gui::TextDirection::LeftBottom:
		t.draw(Arg::bottomLeft(left, bottom), color);
		break;

	case s3d::gui::TextDirection::CenterTop:
		t.draw(Arg::topCenter(centerX, top), color);
		break;

	case s3d::gui::TextDirection::Center:
		t.draw(Arg::center(centerX, centerY), color);
		break;

	case s3d::gui::TextDirection::CenterBottom:
		t.draw(Arg::bottomCenter(centerX, bottom), color);
		break;

	case s3d::gui::TextDirection::RightTop:
		t.draw(Arg::topRight(right, top), color);
		break;

	case s3d::gui::TextDirection::RightCenter:
		t.draw(Arg::rightCenter(right, centerY), color);
		break;

	case s3d::gui::TextDirection::RightBottom:
		t.draw(Arg::bottomRight(right, bottom), color);
		break;
	}
}

void UIText::setPadding(double top, double bottom, double left, double right) {
	paddingTop = top;
	paddingBottom = bottom;
	paddingLeft = left;
	paddingRight = right;
}