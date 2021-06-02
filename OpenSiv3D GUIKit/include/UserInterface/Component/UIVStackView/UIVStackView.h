#pragma once

#include "../UIView/UIView.h"

namespace s3d::gui {
	enum class LeadingDirection {
		Top,
		Bottom
	};

	class UIVStackView : public UIView {
	public:
		bool scrollingEnabled = true;

	private:
		size_t m_maxStackCount = 0;
		double m_rowHeight = 0.0;
		double m_currentRowHeight = 0.0, m_currentRowsHeight = 0.0;
		double m_topPositionConstant = 0.0;
		bool m_constraintsApplied = false;
		LeadingDirection m_leadingDirection = LeadingDirection::Top;

	public:
		using UIView::UIView;

		~UIVStackView();

		virtual void release();

		template<class T>
		void appendComponent(const T& component) {
			T* cmp = new T(component);
			UIView::appendComponent(*cmp);
			m_constraintsApplied = false;
			requestToUpdateLayer();
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

		Array<UIComponent*> components() const {
			return m_userInterfaces;
		}

		void updateLayer() override;

	protected:
		bool mouseWheel() override;

	private:
		void setChildConstraints(size_t index);

		void calcCurrentRowHeight();

		void adjustRowsBottomToViewBottom();
	};
}