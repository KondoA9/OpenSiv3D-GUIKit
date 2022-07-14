#include <Aoba/Layout.hpp>
#include <Aoba/UIView.hpp>

namespace s3d::aoba {
    namespace Internal {
        void LayoutSingle(UIComponent& parent, Layout::Internal::ILayout* const child, bool isHorizontal) {
            auto& component = child->component;

            const auto startingEdge           = isHorizontal ? LayerDirection::Left : LayerDirection::Top;
            const auto endingEdge             = isHorizontal ? LayerDirection::Right : LayerDirection::Bottom;
            const auto sizeDirection          = isHorizontal ? LayerDirection::Width : LayerDirection::Height;
            const auto otherAlignStartingEdge = isHorizontal ? LayerDirection::Top : LayerDirection::Left;
            const auto otherAlignEndingEdge   = isHorizontal ? LayerDirection::Bottom : LayerDirection::Right;

            component.setConstraint(otherAlignStartingEdge, parent, otherAlignStartingEdge);
            component.setConstraint(otherAlignEndingEdge, parent, otherAlignEndingEdge);
            component.setConstraint(startingEdge, parent, startingEdge);

            switch (child->type) {
                using Layout::Internal::ILayout;

            case ILayout::Type::Constant:
                component.setConstraint(sizeDirection, static_cast<Layout::Constant*>(child)->constant);
                break;

            case ILayout::Type::Function:
                component.setConstraint(sizeDirection,
                                        static_cast<Layout::Function*>(child)->func,
                                        static_cast<Layout::Function*>(child)->constant,
                                        static_cast<Layout::Function*>(child)->multiplier);
                break;

            case ILayout::Type::Percent:
                component.setConstraint(
                    sizeDirection, parent, sizeDirection, 0.0, static_cast<Layout::Ratio*>(child)->ratio);
                break;

            case ILayout::Type::Fill:
                component.setConstraint(endingEdge, parent, endingEdge);
                break;
            }
        }

        void LayoutMulti(UIComponent& parent,
                         Array<std::shared_ptr<Layout::Internal::ILayout>>&& children,
                         bool isHorizontal) {
            const auto startingEdge           = isHorizontal ? LayerDirection::Left : LayerDirection::Top;
            const auto endingEdge             = isHorizontal ? LayerDirection::Right : LayerDirection::Bottom;
            const auto sizeDirection          = isHorizontal ? LayerDirection::Width : LayerDirection::Height;
            const auto otherAlignStartingEdge = isHorizontal ? LayerDirection::Top : LayerDirection::Left;
            const auto otherAlignEndingEdge   = isHorizontal ? LayerDirection::Bottom : LayerDirection::Right;

            size_t i            = 0;
            size_t inverseIndex = 0;
            bool inverse        = false;

            while (true) {
                auto& component = children[i]->component;

                // Set other align constraints
                component.setConstraint(otherAlignStartingEdge, parent, otherAlignStartingEdge);
                component.setConstraint(otherAlignEndingEdge, parent, otherAlignEndingEdge);

                // Set starting edge constraint
                if (inverse) {
                    if (i == children.size() - 1) {
                        component.setConstraint(endingEdge, parent, endingEdge);
                    } else {
                        component.setConstraint(endingEdge, children[i + 1]->component, startingEdge);
                    }
                } else {
                    if (i == 0) {
                        component.setConstraint(startingEdge, parent, startingEdge);
                    } else {
                        component.setConstraint(startingEdge, children[i - 1]->component, endingEdge);
                    }
                }

                switch (children[i]->type) {
                    using Layout::Internal::ILayout;

                case ILayout::Type::Constant:
                    component.setConstraint(sizeDirection, static_cast<Layout::Constant*>(children[i].get())->constant);
                    break;

                case ILayout::Type::Function:
                    component.setConstraint(sizeDirection,
                                            static_cast<Layout::Function*>(children[i].get())->func,
                                            static_cast<Layout::Function*>(children[i].get())->constant,
                                            static_cast<Layout::Function*>(children[i].get())->multiplier);
                    break;

                case ILayout::Type::Percent:
                    component.setConstraint(sizeDirection,
                                            parent,
                                            sizeDirection,
                                            0.0,
                                            static_cast<Layout::Ratio*>(children[i].get())->ratio);
                    break;

                case ILayout::Type::Fill:
                    if (inverse) {
                        if (i == children.size() - 1) {
                            component.setConstraint(startingEdge, children[i - 1]->component, endingEdge);
                        } else {
                            component.setConstraint(startingEdge, children[i - 1]->component, endingEdge);
                        }
                    } else {
                        if (i == children.size() - 1) {
                            component.setConstraint(endingEdge, parent, endingEdge);
                        } else if (i == 0) {
                            component.setConstraint(endingEdge, children[i + 1]->component, startingEdge);
                        } else {
                            component.setConstraint(endingEdge, children[i + 1]->component, startingEdge);
                        }
                    }

                    inverseIndex = i;
                    i            = children.size();
                    inverse      = true;

                    break;
                }

                if (inverse) {
                    if (i == 0 || i == inverseIndex + 1) {
                        break;
                    }
                    i--;
                } else {
                    if (i + 1 == children.size()) {
                        break;
                    }
                    i++;
                }
            }
        }

        UIView& Layout(UIView& self, Array<std::shared_ptr<Layout::Internal::ILayout>>&& children, bool isHorizontal) {
            if (!children.isEmpty()) {
                if (children.size() == 1) {
                    LayoutSingle(self, children[0].get(), isHorizontal);
                } else {
                    LayoutMulti(self, std::move(children), isHorizontal);
                }
            }
            return self;
        }
    }

    template <>
    UIView& UIView::layout<Layout::AlignHorizontal>(Array<std::shared_ptr<Layout::Internal::ILayout>>&& children) {
        return Internal::Layout(*this, std::move(children), true);
    }

    template <>
    UIView& UIView::layout<Layout::AlignVertical>(Array<std::shared_ptr<Layout::Internal::ILayout>>&& children) {
        return Internal::Layout(*this, std::move(children), false);
    }
}
