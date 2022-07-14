#pragma once

#include "UIComponent.hpp"

namespace s3d::aoba {
    namespace Layout {
        namespace Internal {
            struct ILayout {
                enum class Type { Constant, Fill, Function, Percent };

                const Type type;
                UIComponent& component;

                explicit ILayout(Type _type, UIComponent& _component) : type(_type), component(_component) {}

                virtual ~ILayout() = default;
            };
        }

        struct Constant : public Internal::ILayout {
            const double constant;

            explicit Constant(UIComponent& _component, double _constant) :
                Internal::ILayout(ILayout::Type::Constant, _component), constant(_constant) {}
        };

        struct Fill : public Internal::ILayout {
            explicit Fill(UIComponent& _component) : Internal::ILayout(ILayout::Type::Fill, _component) {}
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

            explicit Ratio(UIComponent& _component, double _ratio) :
                Internal::ILayout(ILayout::Type::Percent, _component), ratio(_ratio) {}
        };

        struct AlignHorizontal {};
        struct AlignVertical {};
    }

    // Layout the component with the constant value
    [[nodiscard]] inline std::shared_ptr<Layout::Constant> LOConstant(UIComponent& component, double constant) {
        return std::make_shared<Layout::Constant>(component, constant);
    }

    // Layout the component with filling
    [[nodiscard]] inline std::shared_ptr<Layout::Fill> LOFill(UIComponent& component) {
        return std::make_shared<Layout::Fill>(component);
    }

    // Layout the component with the return value of the function
    [[nodiscard]] inline std::shared_ptr<Layout::Function> LOFunction(UIComponent& component,
                                                                      const std::function<double()>& func,
                                                                      double constant   = 0.0,
                                                                      double multiplier = 1.0) {
        return std::make_shared<Layout::Function>(component, func, constant, multiplier);
    }

    // Layout the component with the ratio to the parent component
    [[nodiscard]] inline std::shared_ptr<Layout::Ratio> LORatio(UIComponent& component, double ratio) {
        return std::make_shared<Layout::Ratio>(component, ratio);
    }

    using AlignH = Layout::AlignHorizontal;
    using AlignV = Layout::AlignVertical;

    using LayoutType = std::shared_ptr<Layout::Internal::ILayout>;
}
