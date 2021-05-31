#include "UIVStackView.h"

using namespace s3d::gui;

UIVStackView::~UIVStackView() {
	for (auto& ui : m_userInterfaces) {
		delete ui;
		ui = nullptr;
	}
}

void UIVStackView::release() {
	for (auto& ui : m_userInterfaces) {
		delete ui;
		ui = nullptr;
	}
	m_userInterfaces.release();
	m_topPositionConstant = 0.0;
	// m_layer.top.setConstraint(0.0);
}

void UIVStackView::updateLayer() {
	UIView::updateLayer();
	calcCurrentRowHeight();
	adjustRowsBottomToViewBottom();
}

bool UIVStackView::mouseWheel() {
	if (scrollingEnabled && UIView::mouseWheel()) {
		if (m_currentRowsHeight >= m_layer.height.value) {
			m_topPositionConstant -= Mouse::Wheel() * 40;
			m_topPositionConstant = m_topPositionConstant > 0.0 ? 0.0 : m_topPositionConstant;
			m_layer.top.setConstraint(m_topPositionConstant);
			updateLayer();
			return true;
		}
	}
	return false;
}

void UIVStackView::setChildConstraints(UIComponent* component) {
	const size_t i = m_userInterfaces.size();

	if (m_rowHeight == 0.0) {
		component->setConstraint(LayerDirection::Top, [this, i]() {
			return m_layer.top.value + i * m_layer.height.value / (m_maxStackCount == 0 ? m_userInterfaces.size() : m_maxStackCount);
			});
		component->setConstraint(LayerDirection::Height, [this]() {
			return m_layer.height.value / (m_maxStackCount == 0 ? m_userInterfaces.size() : m_maxStackCount);
			});
	}
	else {
		component->setConstraint(LayerDirection::Top, [this, i]() { return m_layer.top.value + i * m_rowHeight; });
		component->setConstraint(LayerDirection::Height, m_rowHeight);
	}
	component->setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	component->setConstraint(LayerDirection::Right, *this, LayerDirection::Right);
}

void UIVStackView::calcCurrentRowHeight() {
	const size_t rows = m_maxStackCount == 0 ? m_userInterfaces.size() : m_maxStackCount;
	m_currentRowHeight = m_rowHeight == 0.0 ? m_layer.height.value / rows : m_rowHeight;
	m_currentRowsHeight = m_currentRowHeight * rows;
}

void UIVStackView::adjustRowsBottomToViewBottom() {
	if (m_currentRowsHeight < m_layer.height.value) {
		const double tmp = m_layer.height.value - m_currentRowsHeight;
		const double t = m_topPositionConstant + tmp;
		if (t < 0.0) {
			m_topPositionConstant = t;
			m_layer.top.setConstraint(t);
			updateLayer();
		}
	}
}