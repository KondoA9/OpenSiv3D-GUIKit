#include <GUIKit/UIView.h>
#include <GUIKit/WindowManager.h>

using namespace s3d::gui;

void UIView::updateLayer() {
	if (!exist) {
		return;
	}

	UIRect::updateLayer();
	for (auto& ui : m_userInterfaces) {
		ui->updateLayer();
	}
}

void UIView::updateLayerInvert() {
	if (!exist) {
		return;
	}

	for (int i = static_cast<int>(m_userInterfaces.size()) - 1; i >= 0; i--) {
		m_userInterfaces[i]->updateLayer();
	}
	UIRect::updateLayer();
}

bool UIView::updateLayerIfNeeded() {
	if (!exist) {
		return false;
	}

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
		if (ui->drawable()) {
			ui->draw();
		}
	}
}

void UIView::update() {
	if (!exist || !controllable) {
		return;
	}

	UIRect::update();

	for (auto& ui : m_userInterfaces) {
		ui->update();
	}
}

void UIView::updateMouseEvent() {
	if (!exist || !controllable) {
		return;
	}

	UIRect::updateMouseEvent();

	for (auto& ui : m_userInterfaces) {
		ui->updateMouseEvent();
	}
}