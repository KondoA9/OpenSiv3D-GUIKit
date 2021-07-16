#include <GUIKit/UIView.h>
#include <GUIKit/WindowManager.h>
#include <GUIKit/GUIFactory.h>

using namespace s3d::gui;

void UIView::release() {
	UIRect::release();

	for (auto& component : m_components) {
		component->release();
		GUIFactory::RequestReleaseComponent(component->id());
		component.reset();
	}

	m_components.release();

	GUIFactory::ReleaseInvalidComponents();
}

void UIView::appendComponent(const UIComponent& component) {
	if (!m_components.includes_if([&component](const std::shared_ptr<UIComponent>& component2) {
		return component.id() == component2->id();
		})) {

		m_components.push_back(std::move(GUIFactory::GetComponent(component.id())));
	}
}

void UIView::updateLayer(const Rect& scissor) {
	updateScissorRect(scissor);

	UIRect::updateLayer(scissor);

	for (const auto& component : m_components) {
		if (component->exist) {
			component->updateLayer(m_scissorRect);
		}
	}
}

void UIView::updateLayerInvert(const Rect& scissor) {
	updateScissorRect(scissor);

	for (int i = static_cast<int>(m_components.size()) - 1; i >= 0; i--) {
		if (m_components[i]->exist) {
			m_components[i]->updateLayer(m_scissorRect);
		}
	}

	UIRect::updateLayer(scissor);
}

bool UIView::updateLayerIfNeeded(const Rect& scissor) {
	if (UIRect::updateLayerIfNeeded(scissor)) {
		updateScissorRect(scissor);
		for (const auto ui : m_components) {
			if (ui->exist) {
				ui->updateLayer(m_scissorRect);
			}
		}
		return true;
	}
	else {
		bool updated = false;
		for (const auto& component : m_components) {
			if (component->exist) {
				updated |= component->updateLayerIfNeeded(m_scissorRect);
			}
		}
		return updated;
	}
}

void UIView::draw() {
	Graphics2D::SetScissorRect(m_parentScissorRect);

	UIRect::draw();

	Graphics2D::SetScissorRect(m_scissorRect);

	for (const auto& component : m_components) {
		if (component->drawable()) {
			component->draw();
		}
	}

	Graphics2D::SetScissorRect(m_parentScissorRect);
}

void UIView::updateMouseIntersection() {
	UIRect::updateMouseIntersection();

	for (const auto& component : m_components) {
		if (component->updatable()) {
			component->updateMouseIntersection();
		}
	}
}

void UIView::updateInputEvents() {
	UIRect::updateInputEvents();

	for (const auto& component : m_components) {
		if (component->updatable()) {
			component->updateInputEvents();
		}
	}
}

void UIView::updateScissorRect(const Rect& parentScissorRect) {
	const auto left = Max(parentScissorRect.x, static_cast<int>(rect().rect.x));
	const auto top = Max(parentScissorRect.y, static_cast<int>(rect().rect.y));
	const auto right = Min(parentScissorRect.x + parentScissorRect.w, static_cast<int>(rect().rect.x + rect().rect.w));
	const auto bottom = Min(parentScissorRect.y + parentScissorRect.h, static_cast<int>(rect().rect.y + rect().rect.h));
	m_parentScissorRect = parentScissorRect;
	m_scissorRect = Rect(left, top, right - left, bottom - top);
}
