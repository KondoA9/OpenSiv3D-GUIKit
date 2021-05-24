#pragma once

#include "UIRect.h"

namespace s3d::gui {
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
		SDFFont m_font;

	public:
		String text;
		TextDirection direction;

	public:
		UIText(const String& _text, TextDirection _direction, const ColorTheme& _backgroundColor = DynamicColor::clear) :
			UIRect(_backgroundColor),
			text(_text),
			direction(_direction)
		{
			m_font = SDFFont(60, Typeface::Light);
		}

		void draw() override {
			UIRect::draw();

			const auto f = m_font(text);
			Graphics2D::SetSDFParameters(f.font.pixelRange());

			const double fontsize = rect.h * 0.8;
			const Color color = DynamicColor::text;

			switch (direction)
			{
			case s3d::gui::TextDirection::LeftTop:
				f.draw(fontsize, Arg::topLeft(rect.x, rect.y), color);
				break;

			case s3d::gui::TextDirection::LeftCenter:
				f.draw(fontsize, Arg::leftCenter(rect.leftCenter()), color);
				break;

			case s3d::gui::TextDirection::LeftBottom:
				f.draw(fontsize, Arg::bottomLeft(rect.x, rect.y + rect.h), color);
				break;

			case s3d::gui::TextDirection::CenterTop:
				f.draw(fontsize, Arg::topCenter(rect.topCenter()), color);
				break;

			case s3d::gui::TextDirection::Center:
				f.drawAt(fontsize, rect.center(), color);
				break;

			case s3d::gui::TextDirection::CenterBottom:
				f.draw(fontsize, Arg::bottomCenter(rect.bottomCenter()), color);
				break;

			case s3d::gui::TextDirection::RightTop:
				f.draw(fontsize, Arg::topRight(rect.x + rect.w, rect.y), color);
				break;

			case s3d::gui::TextDirection::RightCenter:
				f.draw(fontsize, Arg::rightCenter(rect.rightCenter()), color);
				break;

			case s3d::gui::TextDirection::RightBottom:
				f.draw(fontsize, Arg::bottomRight(rect.x + rect.h, rect.y + rect.h), color);
				break;
			}
		}
	};
}