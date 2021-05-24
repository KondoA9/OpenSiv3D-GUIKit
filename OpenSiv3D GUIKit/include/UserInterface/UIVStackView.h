#pragma once

#include "UIView.h"

namespace s3d::gui {
	class UIVStackView : public UIView {
		size_t m_maxStackCount = 0;
		double m_rowHeight = 0.0;

	public:
		using UIView::UIView;

		~UIVStackView() {
			for (auto& ui : userInterfaces) {
				delete ui;
				ui = nullptr;
			}
		}

		template<class T>
		void appendComponent(T& component) {
			auto cmp = new T(component);

			const auto i = userInterfaces.size();

			
			if (m_rowHeight == 0.0) {
				cmp->setConstraint(LayerDirection::Top, [this, i]() {
					return layer.top.value + i * layer.height.value / (m_maxStackCount == 0 ? userInterfaces.size() : m_maxStackCount);
					});
				cmp->setConstraint(LayerDirection::Height, [this]() {
					return layer.height.value / (m_maxStackCount == 0 ? userInterfaces.size() : m_maxStackCount);
					});
			}
			else {
				cmp->setConstraint(LayerDirection::Top, [this, i]() { return layer.top.value + i * m_rowHeight; });
				cmp->setConstraint(LayerDirection::Height, m_rowHeight);
			}
			cmp->setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
			cmp->setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

			UIView::appendComponent(*cmp);
		}

		void setMaxStackCount(size_t count) {
			m_maxStackCount = count;
		}

		void setRowHeight(double h) {
			m_rowHeight = h;
		}

		void release() {
			for (auto& ui : userInterfaces) {
				delete ui;
				ui = nullptr;
			}
			userInterfaces.release();
		}
	};
}