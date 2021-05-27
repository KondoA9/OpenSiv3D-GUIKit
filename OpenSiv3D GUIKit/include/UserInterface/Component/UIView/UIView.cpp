#include "UIView.h"
#include "../../../Window/WindowManager.h"

using namespace s3d::gui;

void UIView::updateLayer() {
	UIRect::updateLayer();
	for (auto& ui : userInterfaces) {
		ui->updateLayer();
	}
}

void UIView::updateLayerInvert() {
	for (int i = static_cast<int>(userInterfaces.size()) - 1; i >= 0; i--) {
		userInterfaces[i]->updateLayer();
	}
	UIRect::updateLayer();
}

bool UIView::updateLayerIfNeeded() {
	bool updated = false;
	updated |= UIRect::updateLayerIfNeeded();
	for (auto& ui : userInterfaces) {
		updated |= ui->updateLayerIfNeeded();
	}

	return updated;
}

void UIView::draw() {
	if (!drawable()) {
		return;
	}

	UIRect::draw();
	for (auto& ui : userInterfaces) {
		ui->draw();
	}
}

void UIView::updateMouseEvent() {
	UIRect::updateMouseEvent();
	for (auto& ui : userInterfaces) {
		ui->updateMouseEvent();
	}
}