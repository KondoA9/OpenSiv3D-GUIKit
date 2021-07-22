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
		ColorTheme textColor = DynamicColor::Text;

	private:
		String m_text = U"";
		Font m_font = UnifiedFont::Get(UnifiedFontStyle::Medium);
		TextDirection m_direction = TextDirection::LeftCenter;
		DrawableText m_drawableText;
		RectF m_textRegion, m_drawableRegion;
		double paddingTop = 0.0, paddingBottom = 0.0, paddingLeft = 0.0, paddingRight = 0.0;

	public:
		explicit UIText(const ColorTheme& backgroundColor = DynamicColor::Clear, const ColorTheme& _textColor = DynamicColor::Text) noexcept :
			UIRect(backgroundColor)
		{
			textColor = _textColor;
		}

		const Font& font() const {
			return m_font;
		}

		const String& text() const {
			return m_text;
		}

		// Text region is depend on layer.
		// Be careful when use this func in constraint.
		const RectF& textRegion() const {
			return m_textRegion;
		}

		const RectF& drawableRegion() const {
			return m_drawableRegion;
		}

		const UIText& setPadding(double top, double bottom, double left, double right);

		const UIText& setFont(UnifiedFontStyle style);

		const UIText& setText(const String& text);

		const UIText& setDirection(TextDirection direction);

	protected:
		void updateLayer(const Rect& scissor) override;

		void draw() override;

		virtual void updateDrawableText(bool updateField = false);

	private:
		void updateTextRegion();

		void fitTextRegionToRect();

		void updateDrawableRegion();
	};
}
