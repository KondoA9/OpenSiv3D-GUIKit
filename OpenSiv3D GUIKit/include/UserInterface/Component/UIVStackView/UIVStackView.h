#pragma once

#include "../UIView/UIView.h"

namespace s3d::gui {
	class UIVStackView : public UIView {
	public:
		bool scrollingEnabled = true;

	private:
		size_t m_maxStackCount = 0;
		double m_rowHeight = 0.0;
		double m_currentRowHeight = 0.0, m_currentRowsHeight = 0.0;
		double m_topPositionConstant = 0.0;

	public:
		using UIView::UIView;

		~UIVStackView();

		virtual void release();

		template<class T>
		void appendComponent(const T& component) {
			T* cmp = new T(component);
			setChildConstraints(cmp);
			UIView::appendComponent(*cmp);
		}

		void setMaxStackCount(size_t count) {
			m_maxStackCount = count;
		}

		void setRowHeight(double h) {
			m_rowHeight = h;
		}

		Array<UIComponent*> components() const {
			return m_userInterfaces;
		}

		void updateLayer() override;

	protected:
		bool mouseWheel() override;

	private:
		void setChildConstraints(UIComponent* component);

		void calcCurrentRowHeight();

		void adjustRowsBottomToViewBottom();
	};
}