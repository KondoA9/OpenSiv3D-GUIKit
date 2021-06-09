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
		TextDirection direction;
		ColorTheme textColor;

	protected:
		Font m_font;
		double paddingTop = 0.0, paddingBottom = 0.0, paddingLeft = 0.0, paddingRight = 0.0;
		RectF m_drawingRect;

	public:
		UIText(const ColorTheme& backgroundColor = DynamicColor::Clear, const ColorTheme& textColor = DynamicColor::Text)
			:UIText(U"", UnifiedFontStyle::Medium, TextDirection::LeftCenter, backgroundColor, textColor)
		{}

		UIText(const String& text, UnifiedFontStyle style, TextDirection direction, const ColorTheme& backgroundColor = DynamicColor::Clear, const ColorTheme& textColor = DynamicColor::Text) :
			UIRect(backgroundColor),
			text(text),
			direction(direction),
			textColor(textColor),
			m_font(UnifiedFont::Get(style))
		{}

		UIText(UnifiedFontStyle style, TextDirection direction, const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme& textColor = DynamicColor::Text) :
			UIText(U"", style, direction, backgroundColor, textColor)
		{}

		void draw() override;

		void setPadding(double top, double bottom, double left, double right);
	};
}