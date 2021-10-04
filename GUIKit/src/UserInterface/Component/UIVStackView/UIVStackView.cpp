#include <GUIKit/UIVStackView.hpp>

namespace s3d::gui {
	void UIVStackView::initialize() {
		UIView::initialize();

		addEventListener<MouseEvent::Wheel>([this](const MouseEvent::Wheel& e) {
			scroll(e.wheel * 40);
			}, true);

		addEventListener<MouseEvent::LeftDragging>([this](const MouseEvent::LeftDragging& e) {
			scroll(e.previousPos.y - e.pos.y);
			}, true);
	}

	void UIVStackView::release() {
		UIView::release();

		m_leadingPositionConstant = 0.0;

		calcCurrentRowHeight();
	}

	void UIVStackView::onAfterComponentAppended() {
		m_constraintsApplied = false;
		requestToUpdateLayer();
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

	void UIVStackView::updateChildrenConstraints(bool reset) {
		for (size_t i : step(componentsCount())) {
			auto& component = getComponent(i);
			if (reset) {
				component.removeAllConstraints();
			}

			{
				const auto d1 = m_leadingDirection == LeadingDirection::Top ? LayerDirection::Top : LayerDirection::Bottom;
				const auto d2 = m_leadingDirection == LeadingDirection::Top ? LayerDirection::Bottom : LayerDirection::Top;
				if (i == 0) {
					component.setConstraint(d1, *this, d1, m_leadingPositionConstant);
				}
				else {
					component.setConstraint(d1, getComponent(i - 1), d2);
				}
			}

			if (m_rowHeight == 0.0) {
				component.setConstraint(LayerDirection::Height, *this, LayerDirection::Height, 0.0, 1.0 / (m_maxStackCount == 0 ? componentsCount() : m_maxStackCount));
			}
			else {
				component.setConstraint(LayerDirection::Height, m_rowHeight);
			}

			component.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
			component.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);
		}
	}

	void UIVStackView::calcCurrentRowHeight() {
		const size_t rows = m_maxStackCount == 0 ? componentsCount() : m_maxStackCount;
		m_currentRowHeight = m_rowHeight == 0.0 ? layer().height / rows : m_rowHeight;
		m_currentRowsHeight = m_currentRowHeight * rows;
	}

	void UIVStackView::adjustRowsTrailingToViewBottom() {
		if (componentsCount() != 0 && layer().height < m_currentRowsHeight && (
			(m_leadingDirection == LeadingDirection::Top && layer().top + m_leadingPositionConstant + m_currentRowsHeight < layer().bottom) ||
			(m_leadingDirection == LeadingDirection::Bottom && layer().bottom + m_leadingPositionConstant - m_currentRowsHeight > layer().top)))
		{
			if (m_leadingDirection == LeadingDirection::Top) {
				m_leadingPositionConstant = layer().height - m_currentRowsHeight;
			}
			else {
				m_leadingPositionConstant = -layer().height + m_currentRowsHeight;
			}

			updateChildrenConstraints();
		}
	}

	void UIVStackView::scroll(double dy) {
		if (scrollingEnabled && m_currentRowsHeight >= layer().height) {
			m_leadingPositionConstant -= dy;

			if (m_leadingDirection == LeadingDirection::Top) {
				m_leadingPositionConstant = m_leadingPositionConstant > 0.0 ? 0.0 : m_leadingPositionConstant;
			}
			else {
				m_leadingPositionConstant = m_leadingPositionConstant < 0.0 ? 0.0 : m_leadingPositionConstant;
			}

			updateChildrenConstraints();
			adjustRowsTrailingToViewBottom();
			requestToUpdateLayer();
		}
	}
}
