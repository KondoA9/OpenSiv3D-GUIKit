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
		ColorTheme textColor;

	protected:
		String m_text;
		Font m_font;
		TextDirection m_direction;
		double paddingTop = 0.0, paddingBottom = 0.0, paddingLeft = 0.0, paddingRight = 0.0;
		RectF m_drawingRect;
		Size m_textRegion;

	private:
		DrawableText m_drawableText;

	public:
		UIText(const String& text, 
			UnifiedFontStyle style = UnifiedFontStyle::Medium, TextDirection direction = TextDirection::LeftCenter,
			const ColorTheme& backgroundColor = DynamicColor::Clear, const ColorTheme& textColor = DynamicColor::Text) :
			UIRect(backgroundColor),
			m_text(text),
			m_direction(direction),
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

		String text() const {
			return m_text;
		}

		Size textRegion() const {
			return m_textRegion;
		}

		void setPadding(double top, double bottom, double left, double right);

		void setFont(UnifiedFontStyle style);

		void setText(const String& text);

		void setDirection(TextDirection direction);

	protected:
		void updateLayer(const Rect& scissor) override;

		void draw() override;

	private:
		void updateDrawableText();
	};
}
