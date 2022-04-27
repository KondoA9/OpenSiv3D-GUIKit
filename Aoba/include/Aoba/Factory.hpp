#pragma once

#include <Siv3D.hpp>

#include "Core.hpp"
#include "UIView.hpp"

namespace s3d::aoba {
    class Page;
    class UIComponent;

    class Factory {
        friend Page;

    private:
        size_t m_id = 0;

    public:
        Factory(const Factory&) = delete;

        Factory(Factory&&) = delete;

        Factory& operator=(const Factory&) = delete;

        Factory& operator=(Factory&&) = delete;

        template <class T>
        [[nodiscard]] static T& Create(UIView& parent) {
            auto& component = Factory::CreateComponent<T>();

            parent.appendComponent(component);
            parent.onAfterComponentAppended();

            return component;
        }

        template <class T>
        [[nodiscard]] static T& Create(UIView* parent) {
            return Factory::Create<T>(*parent);
        }

        // Create component without parent component
        template <class T>
        [[nodiscard]] static T& Create() {
            auto& component = Factory::CreateComponent<T>();
            Core::AppendIsolatedComponent(component);
            return component;
        }

    private:
        Factory() = default;

        ~Factory() = default;

        static Factory& Instance();

        static void LogCreatedComponent(size_t id, const std::type_info& info);

        template <class T>
        [[nodiscard]] static T& CreateComponent() {
            const size_t id = Instance().createId();

#if SIV3D_BUILD(DEBUG)
            LogCreatedComponent(id, typeid(T));
#endif

            return *static_cast<T*>(Instance().storeComponent(std::shared_ptr<T>(new T(id))).get());
        }

        size_t createId();

        const std::shared_ptr<UIComponent>& storeComponent(const std::shared_ptr<UIComponent>& component);
    };
}
