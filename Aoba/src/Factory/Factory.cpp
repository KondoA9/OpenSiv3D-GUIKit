#include "Aoba/Factory.hpp"

#include "Aoba/UIComponent.hpp"
#include "src/AobaLog/AobaLog.hpp"
#include "src/ComponentStorage/ComponentStorage.hpp"

namespace s3d::aoba {
    Factory& Factory::Instance() {
        static Factory instance;
        return instance;
    }

    UIView& Factory::_CreatePageView() {
        return CreateComponent<UIView>(false);
    }

    void Factory::LogCreatedComponent(size_t id, const std::type_info& info) {
        AobaLog::Log(AobaLog::Type::Info,
                     U"Factory",
                     U"Create " + Unicode::Widen(std::string(info.name())) + U" " + ToString(id));
    }

    size_t Factory::createId() {
        return Instance().m_id++;
    }

    UIComponent& Factory::storeComponent(std::unique_ptr<UIComponent>&& component) {
        auto& storedComponent = ComponentStorage::Store(std::move(component));

        storedComponent.initialize();

        return storedComponent;
    }

    UIComponent& Factory::storeIsolatedComponent(std::unique_ptr<UIComponent>&& component) {
        auto& storedComponent = ComponentStorage::StoreIsolated(std::move(component));

        storedComponent.initialize();

        return storedComponent;
    }
}
