#pragma once

#include "UIComponent.hpp"

namespace s3d::aoba {
    namespace Layout {
        namespace Internal {
            struct ILayout {
                enum class Type { Constant, Fill, Function, Percent };

                const Type type;
                UIComponent& component;

                explicit ILayout(Type _type, UIComponent& _component) noexcept : type(_type), component(_component) {}

                virtual ~ILayout() = default;
            };

            struct Margin {
                double top = 0.0, bottom = 0.0, left = 0.0, right = 0.0;
            };
        }

        struct Constant : public Internal::ILayout {
            const double constant;

            explicit Constant(UIComponent& _component, double _constant) noexcept :
                Internal::ILayout(ILayout::Type::Constant, _component), constant(_constant) {}
        };

        struct Fill : public Internal::ILayout {
            explicit Fill(UIComponent& _component) noexcept : Internal::ILayout(ILayout::Type::Fill, _component) {}
        };

        struct Function : public Internal::ILayout {
            const std::function<double()> func;
            const double constant;
            const double multiplier;

            explicit Function(UIComponent& _component,
                              const std::function<double()>& _func,
                              double _constant,
                              double _multiplier) :
                Internal::ILayout(ILayout::Type::Function, _component),
                func(_func),
                constant(_constant),
                multiplier(_multiplier) {
                assert(func != nullptr);
            }
        };

        struct Ratio : public Internal::ILayout {
            const double ratio;

            explicit Ratio(UIComponent& _component, double _ratio) noexcept :
                Internal::ILayout(ILayout::Type::Percent, _component), ratio(_ratio) {}
        };

        class LayoutProxy {
        private:
            std::shared_ptr<Internal::ILayout> m_layoutPtr;

            Internal::Margin m_margin = {0};

        public:
            LayoutProxy() = delete;

            LayoutProxy(const std::shared_ptr<Internal::ILayout>& ptr) noexcept : m_layoutPtr(ptr) {}

            LayoutProxy(std::shared_ptr<Internal::ILayout>&& ptr) noexcept : m_layoutPtr(ptr) {}

            LayoutProxy(const LayoutProxy&) = default;

            LayoutProxy(LayoutProxy&&) = default;

            LayoutProxy& operator=(const LayoutProxy&) = default;

            LayoutProxy& operator=(LayoutProxy&&) = default;

            const Internal::Margin& margin() const noexcept {
                return m_margin;
            }

            template <class T>
            const T& as() const {
                return static_cast<const T&>(*m_layoutPtr);
            }

            LayoutProxy& marginTop(double margin) noexcept {
                m_margin.top = margin;
                return *this;
            }

            LayoutProxy& marginBottom(double margin) noexcept {
                m_margin.bottom = margin;
                return *this;
            }

            LayoutProxy& marginLeft(double margin) noexcept {
                m_margin.left = margin;
                return *this;
            }

            LayoutProxy& marginRight(double margin) noexcept {
                m_margin.right = margin;
                return *this;
            }

            LayoutProxy& marginY(double top, double bottom) noexcept {
                return marginTop(top).marginBottom(bottom);
            }

            LayoutProxy& marginX(double left, double right) noexcept {
                return marginLeft(left).marginRight(right);
            }

            LayoutProxy& margin(double top, double bottom, double left, double right) noexcept {
                return marginTop(top).marginBottom(bottom).marginLeft(left).marginRight(right);
            }

            Internal::ILayout* operator->() const noexcept {
                return m_layoutPtr.get();
            }
        };

        struct AlignHorizontal {};
        struct AlignVertical {};
    }

    // Layout the component with the constant value
    [[nodiscard]] inline Layout::LayoutProxy LOConstant(UIComponent& component, double constant) {
        return {std::make_shared<Layout::Constant>(component, constant)};
    }

    // Layout the component with filling
    [[nodiscard]] inline Layout::LayoutProxy LOFill(UIComponent& component) {
        return {std::make_shared<Layout::Fill>(component)};
    }

    // Layout the component with the return value of the function
    [[nodiscard]] inline Layout::LayoutProxy LOFunction(UIComponent& component,
                                                        const std::function<double()>& func,
                                                        double constant   = 0.0,
                                                        double multiplier = 1.0) {
        return {std::make_shared<Layout::Function>(component, func, constant, multiplier)};
    }

    // Layout the component with the ratio to the parent component
    [[nodiscard]] inline Layout::LayoutProxy LORatio(UIComponent& component, double ratio) {
        return {std::make_shared<Layout::Ratio>(component, ratio)};
    }

    using AlignH = Layout::AlignHorizontal;
    using AlignV = Layout::AlignVertical;
}
