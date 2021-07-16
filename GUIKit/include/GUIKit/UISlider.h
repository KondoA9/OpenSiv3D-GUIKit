#pragma once

#include "UIView.h"
#include "UIRect.h"
#include "UICircle.h"
#include "UIText.h"
#include "GUIFactory.h"

namespace s3d::gui {
	class UISlider : public UIView {
	private:
		double m_value = 0.0, m_min = 0.0, m_max = 1.0;
		std::function<void(double value)> m_valueChangedHandler;
		bool m_sliding = false;

		UIRect& ui_railLeft = GUIFactory::Create<UIRect>(), & ui_railRight = GUIFactory::Create<UIRect>();
		UICircle& ui_handle = GUIFactory::Create<UICircle>();
		UIText& ui_text = GUIFactory::Create<UIText>();

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
			ui_text.setText(label);
		}

	protected:
		void initialize() override;

		void updateInputEvents() override;
	};
}
