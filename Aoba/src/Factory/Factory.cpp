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
#if SIV3D_BUILD(DEBUG)
				Logger << U"[Aoba](Destroy) " + Unicode::Widen(std::string(typeid(*component).name())) + U" " + ToString(component->id());
#endif
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

	void Factory::ReleaseComponentsIfNeed() {
		if (m_Instance.m_releaseCounter++; m_Instance.m_releaseCounter == 100) {
			ReleaseInvalidComponents();

			if (m_Instance.m_components.capacity() != m_Instance.m_components.size()) {
				m_Instance.m_components.shrink_to_fit();
			}

			m_Instance.m_releaseCounter = 0;
		}
	}
}
