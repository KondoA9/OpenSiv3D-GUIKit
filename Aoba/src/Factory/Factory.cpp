#include <Aoba/Factory.hpp>

#include "src/ComponentStorage/ComponentStorage.hpp"

namespace s3d::aoba {
	size_t Factory::m_Id = 0, Factory::m_PreviousId = 0;
	Factory Factory::m_Instance;

	size_t Factory::GetId() {
		FMT_ASSERT(m_PreviousId != m_Id, "Make sure you instantiated through Factory::Create()");

		m_PreviousId = m_Id;

		return m_Id;
	}

	std::shared_ptr<UIComponent>& Factory::storeComponent(const std::shared_ptr<UIComponent>& component) {
		ComponentStorage::Store(component);

		return ComponentStorage::Get(component->id());
	}
}
