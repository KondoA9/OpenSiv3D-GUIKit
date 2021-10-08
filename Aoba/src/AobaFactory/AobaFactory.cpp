#include <Aoba/AobaFactory.hpp>

namespace s3d::aoba {
	size_t AobaFactory::m_Id = 0, AobaFactory::m_PreviousId = 0;
	AobaFactory AobaFactory::m_Instance;

	size_t AobaFactory::GetId() {
		FMT_ASSERT(m_PreviousId != m_Id, "Make sure you instantiated through AobaFactory::Create()");

		m_PreviousId = m_Id;

		return m_Id;
	}

	std::shared_ptr<UIComponent>& AobaFactory::GetComponent(size_t id) {
		for (auto& component : m_Instance.m_components) {
			if (component && component->id() == id) {
				return component;
			}
		}

		throw Error{ U"A component with identifier \"{}\" not found."_fmt(id) };
	}

	void AobaFactory::RequestReleaseComponent(size_t id) {
		for (auto& component : m_Instance.m_components) {
			if (component && component->id() == id) {
				component.reset();
				break;
			}
		}
	}

	void AobaFactory::ReleaseInvalidComponents() {
		m_Instance.m_components.remove_if([](const std::shared_ptr<UIComponent>& component) {
			return !component;
			});
	}
}
