#pragma once

#include "../UIView/UIView.h"
#include "../UIRect/UIRect.h"
#include "../UICircle/UICircle.h"
#include "../UIText/UIText.h"

namespace s3d::gui {
	class UISlider : public UIView {
	private:
		double m_value = 0.0, m_min = 0.0, m_max = 1.0;
		std::function<void(double value)> m_valueChangedHandler;
		bool m_initialized = false;

		UIRect railLeft, railRight;
		UICircle handle;
		UIText m_text;

	public:
		UISlider(const String& label, UnifiedFontStyle style, TextDirection direction, const ColorTheme& _backgroundColor = DynamicColor::Clear) :
			UIView(_backgroundColor),
			m_text(UIText(label, style, direction))
		{}

		UISlider(const String & label, UnifiedFontStyle style, const ColorTheme & _backgroundColor = DynamicColor::Clear) :
			UISlider(label, style, TextDirection::LeftBottom, _backgroundColor)
		{}

		UISlider(const String& label, const ColorTheme& _backgroundColor = DynamicColor::Clear) :
			UISlider(label, UnifiedFontStyle::Small, _backgroundColor)
		{}

		UISlider(const ColorTheme& _backgroundColor = DynamicColor::Clear) :
			UISlider(U"", _backgroundColor)
		{}

		void updateLayer() override;

		void draw() override;

		double value() const {
			return m_value;
		}

		void setValueChangedHandler(const std::function<void(double value)>& func) {
			m_valueChangedHandler = func;
		}

		void setValue(double value) {
			m_value = value;
			restrictValue();
			requestToUpdateLayer();
		}

		void setRange(double min, double max) {
			m_min = min;
			m_max = max;
			restrictValue();
			requestToUpdateLayer();
		}

		void setLabel(const String& label) {
			m_text.text = label;
		}

	protected:
		bool mouseLeftDragging() override;

		bool mouseHovered() override;

		bool mouseUnHovered() override;

		bool mouseHovering() override;

	private:
		void initialize();

		void restrictValue();
	};
}