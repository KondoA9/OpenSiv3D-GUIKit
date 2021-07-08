#pragma once

#include "UnifiedFont.h"
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
	public:
		String text;
		TextDirection direction;
		ColorTheme textColor;

	protected:
		Font m_font;
		double paddingTop = 0.0, paddingBottom = 0.0, paddingLeft = 0.0, paddingRight = 0.0;
		RectF m_drawingRect;
		Size m_textRegion;

	public:
		UIText(const String& text, 
			UnifiedFontStyle style = UnifiedFontStyle::Medium, TextDirection direction = TextDirection::LeftCenter,
			const ColorTheme& backgroundColor = DynamicColor::Clear, const ColorTheme& textColor = DynamicColor::Text) :
			UIRect(backgroundColor),
			text(text),
			direction(direction),
			textColor(textColor),
			m_font(UnifiedFont::Get(style))
		{}

		UIText(const String & text, UnifiedFontStyle style, const ColorTheme & backgroundColor, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(text, style, TextDirection::LeftCenter, backgroundColor, textColor)
		{}

		UIText(const String & text, TextDirection direction, const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(text, UnifiedFontStyle::Medium, direction, backgroundColor, textColor)
		{}

		UIText(const String & text, const ColorTheme & backgroundColor, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(text, UnifiedFontStyle::Medium, TextDirection::LeftCenter, backgroundColor, textColor)
		{}

		UIText(UnifiedFontStyle style, TextDirection direction, const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(U"", style, direction, backgroundColor, textColor)
		{}

		UIText(UnifiedFontStyle style, const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(U"", style, TextDirection::LeftCenter, backgroundColor, textColor)
		{}

		UIText(TextDirection direction, const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(U"", UnifiedFontStyle::Medium, direction, backgroundColor, textColor)
		{}

		UIText(const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text)
			:UIText(U"", backgroundColor, textColor)
		{}

		Size textRegion() {
			m_textRegion = m_font(text).region(0, 0).size.asPoint();
			return m_textRegion;
		}

		void setPadding(double top, double bottom, double left, double right);

	protected:
		void draw() override;
	};
}
