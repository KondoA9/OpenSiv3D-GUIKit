#include "ComponentStorage.hpp"

namespace s3d::aoba {
	ComponentStorage ComponentStorage::Instance() {
		static ComponentStorage instance;
		return instance;
	}

	void ComponentStorage::Store() {
		Instance().store();
	}

	void ComponentStorage::Release(size_t id) {
		Instance().release(id);
	}

	void ComponentStorage::store() {

	}

	void ComponentStorage::release(size_t id) {

	}
}