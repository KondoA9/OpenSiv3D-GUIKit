#include <GUIKit/UIVStackView.h>

using namespace s3d::gui;

void UIVStackView::initialize() {
	addEventListener<MouseEvent::Wheel>([this](const MouseEvent::Wheel& e) {
		scroll(e.wheel * 40);
		}, true);

	addEventListener<MouseEvent::LeftDragging>([this](const MouseEvent::LeftDragging& e) {
		scroll(e.previousPos.y - e.pos.y);
		}, true);
}

UIVStackView::~UIVStackView() {
	releaseDeletableComponents();
}

void UIVStackView::release() {
	releaseDeletableComponents();
	m_userInterfaces.release();

	m_leadingPositionConstant = 0.0;

	calcCurrentRowHeight();
}

void UIVStackView::releaseDeletableComponents() {
	for (auto component : m_deletableComponents) {
		delete component;
		component = nullptr;
	}

	m_deletableComponents.release();
}

void UIVStackView::updateLayer(const Rect& scissor) {
	if (!m_constraintsApplied) {
		updateChildrenConstraints();
		m_constraintsApplied = true;
	}

	UIView::updateLayer(scissor);

	calcCurrentRowHeight();

	adjustRowsTrailingToViewBottom();
}

void UIVStackView::updateChildrenConstraints() {
	for (size_t i : step(m_userInterfaces.size())) {
		const auto component = m_userInterfaces[i];

		{
			const auto d1 = m_leadingDirection == LeadingDirection::Top ? LayerDirection::Top : LayerDirection::Bottom;
			const auto d2 = m_leadingDirection == LeadingDirection::Top ? LayerDirection::Bottom : LayerDirection::Top;
			if (i == 0) {
				component->setConstraint(d1, *this, d1, m_leadingPositionConstant);
			}
			else {
				component->setConstraint(d1, *m_userInterfaces[i - 1], d2);
			}
		}

		if (m_rowHeight == 0.0) {
			component->setConstraint(LayerDirection::Height, *this, LayerDirection::Height, 0.0, 1.0 / (m_maxStackCount == 0 ? m_userInterfaces.size() : m_maxStackCount));
		}
		else {
			component->setConstraint(LayerDirection::Height, m_rowHeight);
		}

		component->setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
		component->setConstraint(LayerDirection::Right, *this, LayerDirection::Right);
	}
}

void UIVStackView::calcCurrentRowHeight() {
	const size_t rows = m_maxStackCount == 0 ? m_userInterfaces.size() : m_maxStackCount;
	m_currentRowHeight = m_rowHeight == 0.0 ? m_layer.height / rows : m_rowHeight;
	m_currentRowsHeight = m_currentRowHeight * rows;
}

void UIVStackView::adjustRowsTrailingToViewBottom() {
	if (m_userInterfaces&& m_layer.height < m_currentRowsHeight && (
		m_leadingDirection == LeadingDirection::Top && m_layer.top + m_leadingPositionConstant + m_currentRowsHeight < m_layer.bottom ||
		m_leadingDirection == LeadingDirection::Bottom && m_layer.bottom + m_leadingPositionConstant - m_currentRowsHeight > m_layer.top))
	{
		if (m_leadingDirection == LeadingDirection::Top) {
			m_leadingPositionConstant = m_layer.height - m_currentRowsHeight;
		}
		else {
			m_leadingPositionConstant = -m_layer.height + m_currentRowsHeight;
		}

		updateChildrenConstraints();
	}
}

void UIVStackView::scroll(double dy) {
	if (scrollingEnabled && m_currentRowsHeight >= m_layer.height) {
		m_leadingPositionConstant -= dy;

		if (m_leadingDirection == LeadingDirection::Top) {
			m_leadingPositionConstant = m_leadingPositionConstant > 0.0 ? 0.0 : m_leadingPositionConstant;
		}
		else {
			m_leadingPositionConstant = m_leadingPositionConstant < 0.0 ? 0.0 : m_leadingPositionConstant;
		}

		updateChildrenConstraints();
		adjustRowsTrailingToViewBottom();
	}
}
