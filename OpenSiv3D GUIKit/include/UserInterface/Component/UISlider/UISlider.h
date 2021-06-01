#pragma once

#include "../UIView/UIView.h"
#include "../UIRect/UIRect.h"
#include "../UICircle/UICircle.h"

namespace s3d::gui {
	class UISlider : public UIView {
	public:
		double value = 0.0, min = 0.0, max = 1.0;

	private:
		std::function<void()> m_valueChangedHandler;
		bool m_initialized = false;

		UIRect railLeft, railRight;
		UICircle handle;

	public:
		UISlider(const ColorTheme& _backgroundColor = DynamicColor::Clear):
			UIView(_backgroundColor)
		{}

		void updateLayer() override;

		void draw() override;

		void setValueChangedHandler(const std::function<void()>& func) {
			m_valueChangedHandler = func;
		}

	protected:
		bool mouseLeftDragging() override;

		bool mouseHovered() override;

		bool mouseUnHovered() override;

	private:
		void initialize();
	};
}