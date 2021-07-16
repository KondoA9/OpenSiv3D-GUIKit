#pragma once

#include "UIView.h"

namespace s3d::gui {
	enum class LeadingDirection {
		Top,
		Bottom
	};

	class UIVStackView : public UIView {
	public:
		bool scrollingEnabled = true;

	private:
		LeadingDirection m_leadingDirection = LeadingDirection::Top;

		size_t m_maxStackCount = 0;
		double m_rowHeight = 0.0;

		double m_currentRowHeight = 0.0, m_currentRowsHeight = 0.0;
		double m_leadingPositionConstant = 0.0;

		bool m_constraintsApplied = false;

	public:
		using UIView::UIView;

		void release() override;

		void appendComponent(const UIComponent& component) override;

		template<class T>
		T& appendTemporaryComponent(const T& component) {
			m_constraintsApplied = false;
			requestToUpdateLayer();
			return UIView::appendTemporaryComponent(component);
		}

		void setMaxStackCount(size_t count) {
			m_maxStackCount = count;
		}

		void setRowHeight(double h) {
			m_rowHeight = h;
		}

		void setLeadingDirection(LeadingDirection direction) {
			m_leadingDirection = direction;
		}

	protected:
		void initialize() override;

		void updateLayer(const Rect& scissor) override;

	private:
		void updateChildrenConstraints();

		void calcCurrentRowHeight();

		void adjustRowsTrailingToViewBottom();

		void scroll(double dy);
	};
}
