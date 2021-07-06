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

	for (const auto ui : m_userInterfaces) {
		if (ui->exist) {
			ui->updateLayer();
		}
	}

	updateScissorRect(m_parentScissorRect);
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
	if (UIRect::updateLayerIfNeeded()) {
		for (const auto ui : m_userInterfaces) {
			if (ui->exist) {
				ui->updateLayer();
			}
		}
		return true;
	}
	else {
		bool updated = false;
		for (const auto ui : m_userInterfaces) {
			if (ui->exist) {
				updated |= ui->updateLayerIfNeeded();
			}
		}
		return updated;
	}
}

void UIView::draw(const Rect& scissor) {
	if (m_parentScissorRect != scissor) {
		updateScissorRect(scissor);
	}

	Graphics2D::SetScissorRect(m_parentScissorRect);

	UIRect::draw(m_parentScissorRect);

	Graphics2D::SetScissorRect(m_scissorRect);

	for (const auto ui : m_userInterfaces) {
		if (ui->drawable()) {
			ui->draw(m_scissorRect);
		}
	}

	Graphics2D::SetScissorRect(m_parentScissorRect);
}

void UIView::updateMouseIntersection() {
	UIRect::updateMouseIntersection();

	for (const auto ui : m_userInterfaces) {
		if (ui->updatable()) {
			ui->updateMouseIntersection();
		}
	}
}

void UIView::updateInputEvents() {
	UIRect::updateInputEvents();

	for (const auto ui : m_userInterfaces) {
		if (ui->updatable()) {
			ui->updateInputEvents();
		}
	}
}

void UIView::updateScissorRect(const Rect& parentScissorRect) {
	const auto left = Max(parentScissorRect.x, static_cast<int>(m_rect.rect.x));
	const auto top = Max(parentScissorRect.y, static_cast<int>(m_rect.rect.y));
	const auto right = Min(parentScissorRect.x + parentScissorRect.w, static_cast<int>(m_rect.rect.x + m_rect.rect.w));
	const auto bottom = Min(parentScissorRect.y + parentScissorRect.h, static_cast<int>(m_rect.rect.y + m_rect.rect.h));
	m_parentScissorRect = parentScissorRect;
	m_scissorRect = Rect(left, top, right - left, bottom - top);
}
