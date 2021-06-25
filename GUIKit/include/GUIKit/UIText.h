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
		String label, text;
		TextDirection direction;
		ColorTheme textColor;

	protected:
		Font m_font;
		double paddingTop = 0.0, paddingBottom = 0.0, paddingLeft = 0.0, paddingRight = 0.0;
		RectF m_drawingRect;
		Size m_textRegion;

	public:
		UIText(const String& label, const String& text, 
			UnifiedFontStyle style = UnifiedFontStyle::Medium, TextDirection direction = TextDirection::LeftCenter,
			const ColorTheme& backgroundColor = DynamicColor::Clear, const ColorTheme& textColor = DynamicColor::Text) :
			UIRect(backgroundColor),
			label(label),
			text(text),
			direction(direction),
			textColor(textColor),
			m_font(UnifiedFont::Get(style))
		{
			m_textRegion = m_font(label + text).draw().size.asPoint();
		}

		UIText(const String & text, UnifiedFontStyle style, TextDirection direction, const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(U"", text, style, direction, backgroundColor, textColor)
		{}

		UIText(const String & text, UnifiedFontStyle style,const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(U"", text, style, TextDirection::LeftCenter, backgroundColor, textColor)
		{}

		UIText(const String & text, const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text) :
			UIText(U"", text, UnifiedFontStyle::Medium, TextDirection::LeftCenter, backgroundColor, textColor)
		{}

		UIText(UnifiedFontStyle style, TextDirection direction, const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme& textColor = DynamicColor::Text) :
			UIText(U"", U"", style, direction, backgroundColor, textColor)
		{}

		UIText(const ColorTheme & backgroundColor = DynamicColor::Clear, const ColorTheme & textColor = DynamicColor::Text)
			:UIText(U"", backgroundColor, textColor)
		{}

		Size textRegion() const {
			return m_textRegion;
		}

		void setPadding(double top, double bottom, double left, double right);

	protected:
		void draw() override;
	};
}