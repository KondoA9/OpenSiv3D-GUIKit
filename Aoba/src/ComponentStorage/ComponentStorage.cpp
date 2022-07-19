#include "ComponentStorage.hpp"

#include "src/AobaLog/AobaLog.hpp"

// ComponentStorage::m_components and ComponentStorage::m_isolatedComponents are sorted when a new component is
// inserted. To find the component from m_components or m_isolatedComponents, use binary search. Insertion does not
// occur frequently but searching occurs frequently. So computational complexity is depends on search algorithm.
// Therefore using sorting and binary search not naive method.

#define STORAGE_CONTAINER(ISOLATED) (ISOLATED ? m_isolatedComponents : m_components)

namespace s3d::aoba {
    ComponentStorage& ComponentStorage::Instance() {
        static ComponentStorage instance;
        return instance;
    }

    UIComponent& ComponentStorage::Get(size_t id) {
        if (const auto index = Instance().findComponentById(id, false); index.has_value()) {
            return *Instance().m_components[index.value()];
        }

        if (const auto index = Instance().findComponentById(id, true); index.has_value()) {
            return *Instance().m_isolatedComponents[index.value()];
        }

        // this code should not be called
        throw Error{U"A component with identifier \"{}\" not found."_fmt(id)};
    }

    bool ComponentStorage::Has(size_t id) {
        return Instance().findComponentById(id, false) || Instance().findComponentById(id, true);
    }

    UIComponent& ComponentStorage::Store(std::unique_ptr<UIComponent>&& component) {
        // Insert the component in ascending order
        return Instance().insertComponent(std::move(component), false);
    }

    UIComponent& ComponentStorage::StoreIsolated(std::unique_ptr<UIComponent>&& component) {
        // Insert the component in ascending order
        return Instance().insertComponent(std::move(component), true);
    }

    bool ComponentStorage::Release(size_t id) {
        return Instance().releaseComponent(id, false) || Instance().releaseComponent(id, true);
    }

    Optional<size_t> ComponentStorage::findComponentById(size_t id, bool isolated) {
        if (STORAGE_CONTAINER(isolated).isEmpty()) {
            return none;
        }

        size_t left = 0, right = STORAGE_CONTAINER(isolated).size() - 1;

        while (left <= right) {
            const size_t mid = (left + right) / 2;
            const auto value = STORAGE_CONTAINER(isolated)[mid]->id();

            if (value < id) {
                left = mid + 1;
            } else if (value > id) {
                if (mid == 0) {
                    break;
                }
                right = mid - 1;
            } else {
                return mid;
            }
        }

        return none;
    }

    UIComponent& ComponentStorage::insertComponent(std::unique_ptr<UIComponent>&& component, bool isolated) {
        size_t lowerIndex = 0;

        if (!STORAGE_CONTAINER(isolated).isEmpty()) {
            const size_t key = component->id();

            size_t left = 0, right = STORAGE_CONTAINER(isolated).size() - 1;

            while (left <= right) {
                const size_t mid = (left + right) / 2;
                const auto value = STORAGE_CONTAINER(isolated)[mid]->id();

                if (value < key) {
                    left = mid + 1;
                } else if (value > key) {
                    if (mid == 0) {
                        break;
                    }
                    right = mid - 1;
                } else {
                    lowerIndex = mid;
                }
            }

            lowerIndex = left;
        }

        STORAGE_CONTAINER(isolated).insert(std::next(STORAGE_CONTAINER(isolated).begin(), lowerIndex),
                                           std::move(component));
        return *STORAGE_CONTAINER(isolated)[lowerIndex];
    }

    bool ComponentStorage::releaseComponent(size_t id, bool isolated) {
        const auto index = findComponentById(id, isolated);
        if (index.has_value()) {
#if SIV3D_BUILD(DEBUG)
            const auto& componentRef = *STORAGE_CONTAINER(isolated)[index.value()];
            AobaLog::Log(AobaLog::Type::Info,
                         U"ComponentStorage",
                         U"Destroy {} {}"_fmt(Unicode::Widen(typeid(componentRef).name()),
                                              STORAGE_CONTAINER(isolated)[index.value()]->id()));
#endif
            STORAGE_CONTAINER(isolated)[index.value()].reset();
            STORAGE_CONTAINER(isolated).remove(STORAGE_CONTAINER(isolated)[index.value()]);
            return true;
        }
        return false;
    }
}
