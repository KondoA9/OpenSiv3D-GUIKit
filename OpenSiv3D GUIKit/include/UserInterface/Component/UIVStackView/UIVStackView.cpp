#include "UIVStackView.h"

using namespace s3d::gui;

UIVStackView::~UIVStackView() {
	for (auto& ui : userInterfaces) {
		delete ui;
		ui = nullptr;
	}
}

void UIVStackView::release() {
	for (auto& ui : userInterfaces) {
		delete ui;
		ui = nullptr;
	}
	userInterfaces.release();
}

bool UIVStackView::mouseWheel() {
	if (UIView::mouseWheel()) {
		m_topPositionConstant -= Mouse::Wheel() * 40;
		m_topPositionConstant = Clamp(m_topPositionConstant, -m_layer.height.value, 0.0);
		m_layer.top.setConstraint(m_topPositionConstant);
		requestToUpdateLayer();
		return true;
	}
	return false;
}

void UIVStackView::setChildConstraints(UIComponent* component) {
	const size_t i = userInterfaces.size();

	if (m_rowHeight == 0.0) {
		component->setConstraint(LayerDirection::Top, [this, i]() {
			return m_layer.top.value + i * m_layer.height.value / (m_maxStackCount == 0 ? userInterfaces.size() : m_maxStackCount);
			});
		component->setConstraint(LayerDirection::Height, [this]() {
			return m_layer.height.value / (m_maxStackCount == 0 ? userInterfaces.size() : m_maxStackCount);
			});
	}
	else {
		component->setConstraint(LayerDirection::Top, [this, i]() { return m_layer.top.value + i * m_rowHeight; });
		component->setConstraint(LayerDirection::Height, m_rowHeight);
	}
	component->setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	component->setConstraint(LayerDirection::Right, *this, LayerDirection::Right);
}