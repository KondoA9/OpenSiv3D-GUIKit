#include <GUIKit/GUIFactory.h>

namespace s3d::gui {
	size_t GUIFactory::m_Id = 0, GUIFactory::m_PreviousId = 0;
	GUIFactory GUIFactory::m_Instance;

	size_t GUIFactory::GetId() {
		FMT_ASSERT(m_PreviousId != m_Id, "Make sure you instantiated through GUIFactory::Create()");

		m_PreviousId = m_Id;

		return m_Id;
	}

	std::shared_ptr<UIComponent>& GUIFactory::GetComponent(size_t id) {
		for (auto& component : m_Instance.m_components) {
			if (component && component->id() == id) {
				return component;
			}
		}

		throw "";
	}

	void GUIFactory::RequestReleaseComponent(size_t id) {
		for (auto& component : m_Instance.m_components) {
			if (component && component->id() == id) {
				component.reset();
				break;
			}
		}
	}

	void GUIFactory::ReleaseInvalidComponents() {
		m_Instance.m_components.remove_if([](const std::shared_ptr<UIComponent>& component) {
			return !component;
			});
	}
}
