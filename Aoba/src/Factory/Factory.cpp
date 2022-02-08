#include "Aoba/Factory.hpp"

#include "Aoba/UIComponent.hpp"
#include "src/AobaLog/AobaLog.hpp"
#include "src/ComponentStorage/ComponentStorage.hpp"

namespace s3d::aoba {
    Factory& Factory::Instance() {
        static Factory instance;
        return instance;
    }

    void Factory::LogCreatedComponent(size_t id, const std::type_info& info) {
        AobaLog::Log(AobaLog::Type::Info,
                     U"Factory",
                     U"Create " + Unicode::Widen(std::string(info.name())) + U" " + ToString(id));
    }

    size_t Factory::createId() {
        return Instance().m_id++;
    }

    std::shared_ptr<UIComponent>& Factory::storeComponent(const std::shared_ptr<UIComponent>& component) {
        component->initialize();

        ComponentStorage::Store(component);

        return ComponentStorage::Get(component->id());
    }
}
