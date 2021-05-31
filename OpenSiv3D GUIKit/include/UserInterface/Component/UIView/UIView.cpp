#include "UIView.h"
#include "../../../Window/WindowManager.h"

using namespace s3d::gui;

void UIView::updateLayer() {
	UIRect::updateLayer();
	for (auto& ui : m_userInterfaces) {
		ui->updateLayer();
	}
}

void UIView::updateLayerInvert() {
	for (int i = static_cast<int>(m_userInterfaces.size()) - 1; i >= 0; i--) {
		m_userInterfaces[i]->updateLayer();
	}
	UIRect::updateLayer();
}

bool UIView::updateLayerIfNeeded() {
	bool updated = false;
	updated |= UIRect::updateLayerIfNeeded();
	for (auto& ui : m_userInterfaces) {
		updated |= ui->updateLayerIfNeeded();
	}

	return updated;
}

void UIView::draw() {
	if (!drawable()) {
		return;
	}

	UIRect::draw();
	for (auto& ui : m_userInterfaces) {
		ui->draw();
	}
}

void UIView::updateMouseEvent() {
	UIRect::updateMouseEvent();
	for (auto& ui : m_userInterfaces) {
		ui->updateMouseEvent();
	}
}