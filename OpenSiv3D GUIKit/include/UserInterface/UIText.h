#pragma once

#include "UIRect.h"
#include "Font/UnifiedFont.h"

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
	public:
		String text;
		UnifiedFontStyle style;
		TextDirection direction;

	public:
		UIText(const String& _text, UnifiedFontStyle _style, TextDirection _direction, const ColorTheme& _backgroundColor = DynamicColor::clear) :
			UIRect(_backgroundColor),
			text(_text),
			style(_style),
			direction(_direction)
		{}

		void draw() override {
			UIRect::draw();

			const Color color = DynamicColor::text;
			const auto t = UnifiedFont::Get(style)(text);

			switch (direction)
			{
			case s3d::gui::TextDirection::LeftTop:
				t.draw(Arg::topLeft(rect.x, rect.y), color);
				break;

			case s3d::gui::TextDirection::LeftCenter:
				t.draw(Arg::leftCenter(rect.leftCenter()), color);
				break;

			case s3d::gui::TextDirection::LeftBottom:
				t.draw(Arg::bottomLeft(rect.x, rect.y + rect.h), color);
				break;

			case s3d::gui::TextDirection::CenterTop:
				t.draw(Arg::topCenter(rect.topCenter()), color);
				break;

			case s3d::gui::TextDirection::Center:
				t.drawAt(rect.center(), color);
				break;

			case s3d::gui::TextDirection::CenterBottom:
				t.draw(Arg::bottomCenter(rect.bottomCenter()), color);
				break;

			case s3d::gui::TextDirection::RightTop:
				t.draw(Arg::topRight(rect.x + rect.w, rect.y), color);
				break;

			case s3d::gui::TextDirection::RightCenter:
				t.draw(Arg::rightCenter(rect.rightCenter()), color);
				break;

			case s3d::gui::TextDirection::RightBottom:
				t.draw(Arg::bottomRight(rect.x + rect.h, rect.y + rect.h), color);
				break;
			}
		}
	};
}