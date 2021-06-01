#include "UISlider.h"

using namespace s3d::gui;

void UISlider::updateLayer() {
	if (!m_initialized) {
		initialize();
		m_initialized = true;
	}

	UIView::updateLayer();
}

void UISlider::draw() {
	if (!drawable()) {
		return;
	}

	UIView::draw();
}

bool UISlider::mouseLeftDragging() {
	if (UIView::mouseLeftDragging()) {
		const double pre = m_value;
		m_value = m_min + (m_max - m_min) * (Cursor::Pos().x - m_layer.left.value) / m_layer.width.value;
		restrictValue();
		if (pre != m_value) {
			requestToUpdateLayer();
			if (m_valueChangedHandler) {
				m_valueChangedHandler(m_value);
			}
			return true;
		}
	}
	return false;
}

bool UISlider::mouseHovered() {
	if (UIView::mouseHovered()) {
		handle.backgroundColor = DynamicColor::DefaultBlue;
		return true;
	}
	return false;
}

bool UISlider::mouseUnHovered() {
	if (UIView::mouseUnHovered()) {
		handle.backgroundColor = DynamicColor::Background;
		return true;
	}
	return false;
}

bool UISlider::mouseHovering() {
	if (UIView::mouseHovering()) {
		Cursor::RequestStyle(CursorStyle::Hand);
		return true;
	}
	return false;
}

void UISlider::initialize() {
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

	const double r = 20;
	handle.drawFrame = true;
	handle.backgroundColor = DynamicColor::Background;
	handle.setConstraint(LayerDirection::CenterY, *this, LayerDirection::CenterY);
	handle.setConstraint(LayerDirection::Height, r);
	handle.setConstraint(LayerDirection::CenterX, [this] {
		return m_layer.left.value + m_layer.width.value * (m_value - m_min) / (m_max - m_min);
		});
	handle.setConstraint(LayerDirection::Width, r);

	appendComponent(railLeft);
	appendComponent(railRight);
	appendComponent(handle);
}

void UISlider::restrictValue() {
	if (m_value < m_min)m_value = m_min;
	if (m_value > m_max)m_value = m_max;
}