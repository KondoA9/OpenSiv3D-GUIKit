#include "ComponentStorage.hpp"

#include "src/Algorithm/Algorithm.hpp"
#include "src/AobaLog/AobaLog.hpp"

// ComponentStorage::m_components and ComponentStorage::m_isolatedComponents are sorted when a new component is
// inserted. To find the component from m_components or m_isolatedComponents, use binary search. Insertion does not
// occur frequently but searching occurs frequently. So computational complexity is depends on search algorithm.
// Therefore using sorting and binary search not naive method.

namespace s3d::aoba {
    namespace Internal {
        Optional<size_t> FindComponentById(const Array<std::shared_ptr<UIComponent>>& components, size_t id) {
            return Algorithm::FindElement<std::shared_ptr<UIComponent>>(
                components, id, [](const std::shared_ptr<UIComponent>& component) { return component->id(); });
        }

        const std::shared_ptr<UIComponent>& InsertComponent(Array<std::shared_ptr<UIComponent>>& sortedArray,
                                                            const std::shared_ptr<UIComponent>&& component) {
            const auto index = Algorithm::FindLowerElement<std::shared_ptr<UIComponent>>(
                sortedArray, component->id(), [](const std::shared_ptr<UIComponent>& component) {
                    return component->id();
                });

            sortedArray.insert(sortedArray.begin() + index, component);
            return sortedArray[index];
        }

        void ReleaseComponent(Array<std::shared_ptr<UIComponent>>& componentsArray, size_t id) {
            const auto result = FindComponentById(componentsArray, id);
            if (result.has_value()) {
                auto& component = componentsArray[result.value()];
#if SIV3D_BUILD(DEBUG)
                const auto& componentRef = *component.get();
                AobaLog::Log(AobaLog::Type::Info,
                             U"ComponentStorage",
                             U"Destroy {} {}"_fmt(Unicode::Widen(typeid(componentRef).name()), component->id()));
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
        if (const auto& index = Internal::FindComponentById(Instance().m_components, id); index.has_value()) {
            return Instance().m_components[index.value()];
        }

        if (const auto& index = Internal::FindComponentById(Instance().m_isolatedComponents, id); index.has_value()) {
            return Instance().m_isolatedComponents[index.value()];
        }

        // this code should not be called
        throw Error{U"A component with identifier \"{}\" not found."_fmt(id)};
    }

    bool ComponentStorage::Has(size_t id) {
        return Internal::FindComponentById(Instance().m_components, id).has_value()
               || Internal::FindComponentById(Instance().m_isolatedComponents, id).has_value();
    }

    const std::shared_ptr<UIComponent>& ComponentStorage::Store(std::shared_ptr<UIComponent>&& component) {
        // Insert the component in ascending order
        return Internal::InsertComponent(Instance().m_components, std::move(component));
    }

    const std::shared_ptr<UIComponent>& ComponentStorage::StoreIsolated(std::shared_ptr<UIComponent>&& component) {
        // Insert the component in ascending order
        return Internal::InsertComponent(Instance().m_isolatedComponents, std::move(component));
    }

    void ComponentStorage::Release(size_t id) {
        Internal::ReleaseComponent(Instance().m_components, id);
        Internal::ReleaseComponent(Instance().m_isolatedComponents, id);
    }
}
