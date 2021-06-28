#include <GUIKit/UIView.h>
#include <GUIKit/WindowManager.h>

using namespace s3d::gui;

void UIView::appendComponent(UIComponent& ui) {
	if (!m_userInterfaces.includes(&ui)) {
		m_userInterfaces.push_back(&ui);
	}
}

void UIView::updateLayer() {
	UIRect::updateLayer();

	for (auto& ui : m_userInterfaces) {
		if (ui->exist) {
			ui->updateLayer();
		}
	}
}

void UIView::updateLayerInvert() {
	for (int i = static_cast<int>(m_userInterfaces.size()) - 1; i >= 0; i--) {
		if (m_userInterfaces[i]->exist) {
			m_userInterfaces[i]->updateLayer();
		}
	}

	UIRect::updateLayer();
}

bool UIView::updateLayerIfNeeded() {
	bool updated = false;
	updated |= UIRect::updateLayerIfNeeded();

	for (auto& ui : m_userInterfaces) {
		if (ui->exist) {
			updated |= ui->updateLayerIfNeeded();
		}
	}

	return updated;
}

void UIView::draw() {
	UIRect::draw();

	for (auto& ui : m_userInterfaces) {
		if (ui->drawable()) {
			ui->draw();
		}
	}
}

void UIView::updateMouseIntersection() {
	UIRect::updateMouseIntersection();

	for (auto& ui : m_userInterfaces) {
		if (ui->updatable()) {
			ui->updateMouseIntersection();
		}
	}
}

void UIView::updateInputEvents() {
	UIRect::updateInputEvents();

	for (auto& ui : m_userInterfaces) {
		if (ui->updatable()) {
			ui->updateInputEvents();
		}
	}
}