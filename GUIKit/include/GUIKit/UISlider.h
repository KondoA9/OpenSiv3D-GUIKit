#pragma once

#include "UIView.h"
#include "UIRect.h"
#include "UICircle.h"
#include "UIText.h"

namespace s3d::gui {
	class UISlider : public UIView {
	private:
		double m_value = 0.0, m_min = 0.0, m_max = 1.0;
		std::function<void(double value)> m_valueChangedHandler;
		bool m_sliding = false;

		UIRect ui_railLeft, ui_railRight;
		UICircle ui_handle;
		UIText ui_text;

	public:
		UISlider(const String& label, UnifiedFontStyle style, TextDirection direction, const ColorTheme& _backgroundColor = DynamicColor::Clear);

		UISlider(const String & label, UnifiedFontStyle style, const ColorTheme & _backgroundColor = DynamicColor::Clear) :
			UISlider(label, style, TextDirection::LeftBottom, _backgroundColor)
		{}

		explicit UISlider(const String& label, const ColorTheme& _backgroundColor = DynamicColor::Clear) :
			UISlider(label, UnifiedFontStyle::Small, _backgroundColor)
		{}

		explicit UISlider(const ColorTheme& _backgroundColor = DynamicColor::Clear) :
			UISlider(U"", _backgroundColor)
		{}

		double value() const {
			return m_value;
		}

		void setValueChangedHandler(const std::function<void(double value)>& func) {
			m_valueChangedHandler = func;
		}

		void setValue(double value) {
			m_value = Clamp(value, m_min, m_max);
			requestToUpdateLayer();
		}

		void setRange(double min, double max) {
			m_min = min;
			m_max = max;
			m_value = Clamp(m_value, m_min, m_max);
			requestToUpdateLayer();
		}

		void setLabel(const String& label) {
			ui_text.setText(label);
		}

	protected:
		void initialize() override;

		void updateInputEvents() override;
	};
}
