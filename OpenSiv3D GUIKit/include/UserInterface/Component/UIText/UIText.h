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

	protected:
		double paddingTop = 0.0, paddingBottom = 0.0, paddingLeft = 0.0, paddingRight = 0.0;

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

		UIText(UnifiedFontStyle _style, TextDirection _direction, const ColorTheme & _backgroundColor = DynamicColor::Clear) :
			UIText(U"", _style, _direction, _backgroundColor)
		{}

		void draw() override;

		void setPadding(double top, double bottom, double left, double right);
	};
}