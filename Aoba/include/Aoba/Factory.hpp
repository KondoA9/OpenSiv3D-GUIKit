#pragma once

#include <Siv3D.hpp>

#include "Core.hpp"
#include "UIView.hpp"

namespace s3d::aoba {
    class UIComponent;

    class Factory {
    private:
        size_t m_id = 0;

    public:
        Factory(const Factory&) = delete;

        Factory(Factory&&) = delete;

        Factory& operator=(const Factory&) = delete;

        Factory& operator=(Factory&&) = delete;

        template <class T>
        [[nodiscard]] static T& Create(UIView& parent) {
            auto& component = Factory::CreateComponent<T>(false);

            parent.appendComponent(component);
            parent.onAfterComponentAppended();

            return component;
        }

        template <class T>
        [[nodiscard]] static T& Create(UIView* parent) {
            return Factory::Create<T>(*parent);
        }

        // Create isolated component without parent view
        template <class T>
        [[nodiscard]] static T& Create() {
            return Factory::CreateComponent<T>(true);
        }

        // Create root view of page
        // Do not call this function
        [[nodiscard]] static UIView& _CreatePageView();

    private:
        Factory() = default;

        ~Factory() = default;

        static Factory& Instance();

        static void LogCreatedComponent(size_t id, const std::type_info& info);

        template <class T>
        [[nodiscard]] static T& CreateComponent(bool isolated) {
            const size_t id = Instance().createId();

#if SIV3D_BUILD(DEBUG)
            LogCreatedComponent(id, typeid(T));
#endif

            if (isolated) {
                return *static_cast<T*>(Instance().storeIsolatedComponent(std::shared_ptr<T>(new T(id))).get());
            } else {
                return *static_cast<T*>(Instance().storeComponent(std::shared_ptr<T>(new T(id))).get());
            }
        }

        size_t createId();

        const std::shared_ptr<UIComponent>& storeComponent(std::shared_ptr<UIComponent>&& component);

        const std::shared_ptr<UIComponent>& storeIsolatedComponent(std::shared_ptr<UIComponent>&& component);
    };
}
