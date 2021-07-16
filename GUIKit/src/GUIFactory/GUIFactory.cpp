#include <GUIKit/GUIFactory.h>

#include <GUIKit/UIComponent.h>
#include <GUIKit/UIView.h>

using namespace s3d::gui;

size_t GUIFactory::m_Id = 0;
GUIFactory GUIFactory::instance;

size_t GUIFactory::GetId() {
	return m_Id;
}

const std::shared_ptr<UIComponent>& GUIFactory::GetComponent(size_t id) {
	for (const auto& component : instance.m_components) {
		if (component->id() == id) {
			return component;
		}
	}

	assert(false);

	return instance.m_components[0];
}

void GUIFactory::RequestReleaseComponent(size_t id) {
	for (auto& component : instance.m_components) {
		if (component && component->id() == id) {
			component.reset();
			break;
		}
	}
}

void GUIFactory::ReleaseInvalidComponents() {
	instance.m_components.remove_if([](const std::shared_ptr<UIComponent>& component) {
		return !component;
		});
}
