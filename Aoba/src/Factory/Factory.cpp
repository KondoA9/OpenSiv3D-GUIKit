#include <Aoba/Factory.hpp>

namespace s3d::aoba {
	size_t Factory::m_Id = 0, Factory::m_PreviousId = 0;
	Factory Factory::m_Instance;

	size_t Factory::GetId() {
		FMT_ASSERT(m_PreviousId != m_Id, "Make sure you instantiated through Factory::Create()");

		m_PreviousId = m_Id;

		return m_Id;
	}

	std::shared_ptr<UIComponent>& Factory::GetComponent(size_t id) {
		for (auto& component : m_Instance.m_components) {
			if (component && component->id() == id) {
				return component;
			}
		}

		throw Error{ U"A component with identifier \"{}\" not found."_fmt(id) };
	}

	void Factory::RequestReleaseComponent(size_t id) {
		for (auto& component : m_Instance.m_components) {
			if (component && component->id() == id) {
				component.reset();
				break;
			}
		}
	}

	void Factory::ReleaseInvalidComponents() {
		m_Instance.m_components.remove_if([](const std::shared_ptr<UIComponent>& component) {
			return !component;
			});
	}
}
