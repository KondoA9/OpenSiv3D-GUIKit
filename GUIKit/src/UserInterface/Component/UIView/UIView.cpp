#include <GUIKit/UIView.h>
#include <GUIKit/WindowManager.h>

using namespace s3d::gui;

void UIView::appendComponent(UIComponent& ui) {
	if (!m_userInterfaces.includes(&ui)) {
		m_userInterfaces.push_back(&ui);
	}
}

void UIView::updateLayer(const Rect& scissor) {
	updateScissorRect(scissor);

	UIRect::updateLayer(scissor);

	for (const auto ui : m_userInterfaces) {
		if (ui->exist) {
			ui->updateLayer(m_scissorRect);
		}
	}
}

void UIView::updateLayerInvert(const Rect& scissor) {
	updateScissorRect(scissor);

	for (int i = static_cast<int>(m_userInterfaces.size()) - 1; i >= 0; i--) {
		if (m_userInterfaces[i]->exist) {
			m_userInterfaces[i]->updateLayer(m_scissorRect);
		}
	}

	UIRect::updateLayer(scissor);
}

bool UIView::updateLayerIfNeeded(const Rect& scissor) {
	updateScissorRect(scissor);

	if (UIRect::updateLayerIfNeeded(scissor)) {
		for (const auto ui : m_userInterfaces) {
			if (ui->exist) {
				ui->updateLayer(m_scissorRect);
			}
		}
		return true;
	}
	else {
		bool updated = false;
		for (const auto ui : m_userInterfaces) {
			if (ui->exist) {
				updated |= ui->updateLayerIfNeeded(m_scissorRect);
			}
		}
		return updated;
	}
}

void UIView::draw() {
	Graphics2D::SetScissorRect(m_parentScissorRect);

	UIRect::draw();

	Graphics2D::SetScissorRect(m_scissorRect);

	for (const auto ui : m_userInterfaces) {
		if (ui->drawable()) {
			ui->draw();
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
