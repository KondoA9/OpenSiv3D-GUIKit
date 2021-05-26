#include "UIText.h"

using namespace s3d::gui;

void UIText::draw() {
	UIRect::draw();

	const Color color = DynamicColor::Text;
	const auto t = UnifiedFont::Get(style)(text);

	switch (direction)
	{
	case s3d::gui::TextDirection::LeftTop:
		t.draw(Arg::topLeft(m_rect.x, m_rect.y), color);
		break;

	case s3d::gui::TextDirection::LeftCenter:
		t.draw(Arg::leftCenter(m_rect.leftCenter()), color);
		break;

	case s3d::gui::TextDirection::LeftBottom:
		t.draw(Arg::bottomLeft(m_rect.x, m_rect.y + m_rect.h), color);
		break;

	case s3d::gui::TextDirection::CenterTop:
		t.draw(Arg::topCenter(m_rect.topCenter()), color);
		break;

	case s3d::gui::TextDirection::Center:
		t.drawAt(m_rect.center(), color);
		break;

	case s3d::gui::TextDirection::CenterBottom:
		t.draw(Arg::bottomCenter(m_rect.bottomCenter()), color);
		break;

	case s3d::gui::TextDirection::RightTop:
		t.draw(Arg::topRight(m_rect.x + m_rect.w, m_rect.y), color);
		break;

	case s3d::gui::TextDirection::RightCenter:
		t.draw(Arg::rightCenter(m_rect.rightCenter()), color);
		break;

	case s3d::gui::TextDirection::RightBottom:
		t.draw(Arg::bottomRight(m_rect.x + m_rect.h, m_rect.y + m_rect.h), color);
		break;
	}
}