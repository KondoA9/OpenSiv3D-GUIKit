#pragma once

#include "../../Font/UnifiedFont.h"
#include "../UIRect/UIRect.h"

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
		UIText():
			UIRect()
		{}

		UIText(const String& _text, UnifiedFontStyle _style, TextDirection _direction, const ColorTheme& _backgroundColor = DynamicColor::Clear) :
			UIRect(_backgroundColor),
			text(_text),
			style(_style),
			direction(_direction)
		{}

		void draw() override;
	};
}