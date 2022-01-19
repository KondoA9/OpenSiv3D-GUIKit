#pragma once

namespace s3d::aoba {
	class ComponentStorage {
	public:
		static void Store() {
			Instance().store();
		}

		static void Release(size_t id) {
			Instance().release(id);
		}

	private:
		static ComponentStorage Instance();

		void store();

		void release(size_t id);
	};
}