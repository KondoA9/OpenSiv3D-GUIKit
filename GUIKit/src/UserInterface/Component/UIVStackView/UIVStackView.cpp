#include <GUIKit/UIVStackView.h>

using namespace s3d::gui;

void UIVStackView::initialize() {
	addEventListener<MouseEvent::Wheel>([this](const MouseEvent::Wheel& e) {
		scroll(e.wheel * 40);
		});

	addEventListener<MouseEvent::LeftDragging>([this](const MouseEvent::LeftDragging& e) {
		scroll(e.previousPos.y - e.pos.y);
		});
}

UIVStackView::~UIVStackView() {
	releaseDeletableComponents();
}

void UIVStackView::release() {
	releaseDeletableComponents();
	m_userInterfaces.release();
	m_topPositionConstant = 0.0;
	m_layer.top.setConstraint(0.0);
	updateLayer();
}

void UIVStackView::releaseDeletableComponents() {
	for (auto component : m_deletableComponents) {
		delete component;
		component = nullptr;
	}
	m_deletableComponents.release();
}

void UIVStackView::updateLayer() {
	if (!m_constraintsApplied) {
		for (size_t i : step(m_userInterfaces.size())) {
			setChildConstraints(i);
		}
		m_constraintsApplied = true;
	}

	UIView::updateLayer();
	calcCurrentRowHeight();
	adjustRowsBottomToViewBottom();
}

void UIVStackView::setChildConstraints(size_t index) {
	{
		const auto d1 = m_leadingDirection == LeadingDirection::Top ? LayerDirection::Top : LayerDirection::Bottom;
		const auto d2 = m_leadingDirection == LeadingDirection::Top ? LayerDirection::Bottom : LayerDirection::Top;
		if (index == 0) {
			m_userInterfaces[index]->setConstraint(d1, *this, d1);
		}
		else {
			m_userInterfaces[index]->setConstraint(d1, *m_userInterfaces[index - 1], d2);
		}
	}

	if (m_rowHeight == 0.0) {
		m_userInterfaces[index]->setConstraint(LayerDirection::Height, *this, LayerDirection::Height, 0.0, 1.0 / (m_maxStackCount == 0 ? m_userInterfaces.size() : m_maxStackCount));
	}
	else {
		m_userInterfaces[index]->setConstraint(LayerDirection::Height, m_rowHeight);
	}

	m_userInterfaces[index]->setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	m_userInterfaces[index]->setConstraint(LayerDirection::Right, *this, LayerDirection::Right);
}

void UIVStackView::calcCurrentRowHeight() {
	const size_t rows = m_maxStackCount == 0 ? m_userInterfaces.size() : m_maxStackCount;
	m_currentRowHeight = m_rowHeight == 0.0 ? m_layer.height / rows : m_rowHeight;
	m_currentRowsHeight = m_currentRowHeight * rows;
}

void UIVStackView::adjustRowsBottomToViewBottom() {
	if (m_currentRowsHeight < m_layer.height) {
		const double tmp = m_layer.height - m_currentRowsHeight;
		const double t = m_topPositionConstant + tmp;
		if (t < 0.0) {
			m_topPositionConstant = t;
			m_layer.top.setConstraint(t);
			updateLayer();
		}
	}
}

void UIVStackView::scroll(double dy) {
	if (scrollingEnabled && m_currentRowsHeight >= m_layer.height) {
		m_topPositionConstant -= dy;
		m_topPositionConstant = m_topPositionConstant > 0.0 ? 0.0 : m_topPositionConstant;
		m_layer.top.setConstraint(m_topPositionConstant);
		updateLayer();
	}
}