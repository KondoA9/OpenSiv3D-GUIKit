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

		void updateLayer() override {
			if (!m_initialized) {
				initialize();
				m_initialized = true;
			}

			UIView::updateLayer();
		}
		void draw() override {
			if (!drawable()) {
				return;
			}

			UIView::draw();
		}

		void setValueChangedHandler(const std::function<void()>& func) {
			m_valueChangedHandler = func;
		}

	protected:
		bool mouseLeftDragging() override {
			if (UIView::mouseLeftDragging()) {
				value = min + (max - min) * (Cursor::Pos().x - m_layer.left.value) / m_layer.width.value;
				if (value < min)value = min;
				if (value > max)value = max;
				requestToUpdateLayer();
				if (m_valueChangedHandler) {
					m_valueChangedHandler();
				}
				return true;
			}
			return false;
		}

		bool mouseHovered() override {
			if (UIView::mouseHovered()) {
				handle.backgroundColor = DynamicColor::ButtonPushed;
				return true;
			}
			return false;
		}

		bool mouseUnHovered() override {
			if (UIView::mouseUnHovered()) {
				handle.backgroundColor = DynamicColor::Background;
				return true;
			}
			return false;
		}

	private:
		void initialize() {
			const double h = 7.5;
			railLeft.drawFrame = true;
			railLeft.backgroundColor = DynamicColor::DefaultBlue;
			railLeft.setConstraint(LayerDirection::CenterY, *this, LayerDirection::CenterY);
			railLeft.setConstraint(LayerDirection::Height, h);
			railLeft.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
			railLeft.setConstraint(LayerDirection::Right, handle, LayerDirection::CenterX);

			railRight.drawFrame = true;
			railRight.backgroundColor = DynamicColor::BackgroundSecondary;
			railRight.setConstraint(LayerDirection::CenterY, *this, LayerDirection::CenterY);
			railRight.setConstraint(LayerDirection::Height, h);
			railRight.setConstraint(LayerDirection::Left, handle, LayerDirection::CenterX);
			railRight.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

			const double r = 15;
			handle.drawFrame = true;
			handle.backgroundColor = DynamicColor::Background;
			handle.setConstraint(LayerDirection::CenterY, *this, LayerDirection::CenterY);
			handle.setConstraint(LayerDirection::Height, r);
			handle.setConstraint(LayerDirection::CenterX, [this] {
				return m_layer.left.value + m_layer.width.value * value / (max - min);
				});
			handle.setConstraint(LayerDirection::Width, r);

			appendComponent(railLeft);
			appendComponent(railRight);
			appendComponent(handle);
		}
	};
}