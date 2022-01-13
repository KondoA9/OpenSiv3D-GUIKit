#pragma once

#include "Factory.hpp"
#include "UICircle.hpp"
#include "UIRect.hpp"
#include "UIText.hpp"
#include "UIView.hpp"

namespace s3d::aoba {
	class UISlider : public UIView {
	public:
		static Color DefaultAccentColor;

		Color accentColor = DefaultAccentColor;

	private:
		double m_value = 0.0, m_min = 0.0, m_max = 1.0;
		std::function<void(double value)> m_valueChangedHandler;
		bool m_sliding = false;

		UIText& m_uiText = Factory::Create<UIText>(this);
		UIRect& m_uiRailLeft = Factory::Create<UIRect>(this), & m_uiRailRight = Factory::Create<UIRect>(this);
		UICircle& m_uiHandle = Factory::Create<UICircle>(this);

	public:
		using UIView::UIView;

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
			m_uiText.setText(label);
		}

		void setFont(UnifiedFontStyle style) {
			m_uiText.setFont(style);
		}

	protected:
		void initialize() override;

		void updateInputEvents() override;
	};
}
