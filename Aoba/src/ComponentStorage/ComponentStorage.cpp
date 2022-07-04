#include "ComponentStorage.hpp"

#include "src/AobaLog/AobaLog.hpp"

// ComponentStorage::m_components and ComponentStorage::m_isolatedComponents are sorted when a new component is
// inserted. To find the component from m_components or m_isolatedComponents, use binary search. Insertion does not
// occur frequently but searching occurs frequently. So computational complexity is depends on search algorithm.
// Therefore using sorting and binary search not naive method.

namespace s3d::aoba {
    namespace Internal {
        size_t FindIndexToInsert(const Array<std::shared_ptr<UIComponent>>& sortedArray,
                                 const std::shared_ptr<UIComponent>& component) {
            if (sortedArray.isEmpty()) {
                return 0;
            }

            size_t left = 0, right = sortedArray.size() - 1;

            while (left <= right) {
                const size_t mid      = (left + right) / 2;
                const size_t targetId = sortedArray[mid]->id();

                if (targetId == component->id()) {
                    return mid;
                } else if (targetId < component->id()) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }

            return right + 1;
        }

        void InsertComponent(Array<std::shared_ptr<UIComponent>>& sortedArray,
                             const std::shared_ptr<UIComponent>& component) {
            const size_t i = FindIndexToInsert(sortedArray, component);
            sortedArray.insert(sortedArray.begin() + i, component);
        }

        // Search the component from array by using binary search
        Optional<size_t> SearchComponent(Array<std::shared_ptr<UIComponent>>& sortedArray, size_t id) {
            if (sortedArray.isEmpty()) {
                return none;
            }

            size_t left = 0, right = sortedArray.size() - 1;

            while (left < right) {
                const size_t mid      = left + (right - left) / 2;
                const size_t targetId = sortedArray[mid]->id();

                if (targetId < id) {
                    left = mid + 1;
                } else if (targetId > id) {
                    right = mid;
                } else {
                    return mid;
                }
            }

            if (left == right && sortedArray[left]->id() == id) {
                return left;
            }

            return none;
        }

        void ReleaseComponent(Array<std::shared_ptr<UIComponent>>& componentsArray, size_t id) {
            if (const auto& index = SearchComponent(componentsArray, id); index.has_value()) {
                auto& component = componentsArray[index.value()];
#if SIV3D_BUILD(DEBUG)
                AobaLog::Log(AobaLog::Type::Info,
                             U"ComponentStorage",
                             U"Destroy " + Unicode::Widen(std::string(typeid(*component).name())) + U" "
                                 + ToString(component->id()));
#endif
                component.reset();
                componentsArray.remove(component);
                return;
            }
        }
    }

    ComponentStorage& ComponentStorage::Instance() {
        static ComponentStorage instance;
        return instance;
    }

    const std::shared_ptr<UIComponent>& ComponentStorage::Get(size_t id) {
        if (const auto& index = Internal::SearchComponent(Instance().m_components, id); index.has_value()) {
            return Instance().m_components[index.value()];
        }

        if (const auto& index = Internal::SearchComponent(Instance().m_isolatedComponents, id); index.has_value()) {
            return Instance().m_isolatedComponents[index.value()];
        }

        // this code should not be called
        throw Error{U"A component with identifier \"{}\" not found."_fmt(id)};
    }

    bool ComponentStorage::Has(size_t id) {
        return Internal::SearchComponent(Instance().m_components, id).has_value()
               || Internal::SearchComponent(Instance().m_isolatedComponents, id).has_value();
    }

    void ComponentStorage::Store(const std::shared_ptr<UIComponent>& component) {
        // Insert the component in ascending order
        Internal::InsertComponent(Instance().m_components, component);
    }

    void ComponentStorage::StoreIsolated(const std::shared_ptr<UIComponent>& component) {
        // Insert the component in ascending order
        Internal::InsertComponent(Instance().m_isolatedComponents, component);
    }

    void ComponentStorage::Release(size_t id) {
        Internal::ReleaseComponent(Instance().m_components, id);
        Internal::ReleaseComponent(Instance().m_isolatedComponents, id);
    }
}
