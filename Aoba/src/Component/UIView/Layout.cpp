#include <Aoba/UIView.hpp>

namespace s3d::aoba {
    namespace Internal {
        void LayoutSingle(UIComponent& parent, Layout::LayoutProxy&& child, bool isHorizontal) {
            auto& component = child->component;

            const auto startingEdge           = isHorizontal ? LayerDirection::Left : LayerDirection::Top;
            const auto endingEdge             = isHorizontal ? LayerDirection::Right : LayerDirection::Bottom;
            const auto sizeDirection          = isHorizontal ? LayerDirection::Width : LayerDirection::Height;
            const auto otherAlignStartingEdge = isHorizontal ? LayerDirection::Top : LayerDirection::Left;
            const auto otherAlignEndingEdge   = isHorizontal ? LayerDirection::Bottom : LayerDirection::Right;

            component.setConstraint(
                startingEdge, parent, startingEdge, isHorizontal ? child.margin().left : child.margin().top);
            component.setConstraint(otherAlignStartingEdge,
                                    parent,
                                    otherAlignStartingEdge,
                                    isHorizontal ? child.margin().top : child.margin().left);
            component.setConstraint(otherAlignEndingEdge,
                                    parent,
                                    otherAlignEndingEdge,
                                    -(isHorizontal ? child.margin().bottom : child.margin().right));

            switch (child->type) {
                using Layout::Internal::ILayout;

            case ILayout::Type::Constant:
                component.setConstraint(sizeDirection, child.as<Layout::Constant>().constant);
                break;

            case ILayout::Type::Function:
                component.setConstraint(sizeDirection,
                                        child.as<Layout::Function>().func,
                                        child.as<Layout::Function>().constant,
                                        child.as<Layout::Function>().multiplier);
                break;

            case ILayout::Type::Percent:
                component.setConstraint(sizeDirection, parent, sizeDirection, 0.0, child.as<Layout::Ratio>().ratio);
                break;

            case ILayout::Type::Fill:
                component.setConstraint(
                    endingEdge, parent, endingEdge, -(isHorizontal ? child.margin().right : child.margin().bottom));
                break;
            }
        }

        void LayoutMulti(UIComponent& parent, Array<Layout::LayoutProxy>&& children, bool isHorizontal) {
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
                component.setConstraint(otherAlignStartingEdge,
                                        parent,
                                        otherAlignStartingEdge,
                                        isHorizontal ? children[i].margin().top : children[i].margin().left);
                component.setConstraint(otherAlignEndingEdge,
                                        parent,
                                        otherAlignEndingEdge,
                                        -(isHorizontal ? children[i].margin().bottom : children[i].margin().right));

                // Set starting edge constraint
                if (inverse) {
                    if (i == children.size() - 1) {
                        component.setConstraint(
                            endingEdge,
                            parent,
                            endingEdge,
                            -(isHorizontal ? children[i].margin().right : children[i].margin().bottom));
                    } else {
                        const auto margin = isHorizontal
                                                ? Abs(children[i + 1].margin().left - children[i].margin().right)
                                                : Abs(children[i + 1].margin().top - children[i].margin().bottom);
                        component.setConstraint(endingEdge, children[i + 1]->component, startingEdge, -margin);
                    }
                } else {
                    if (i == 0) {
                        component.setConstraint(startingEdge,
                                                parent,
                                                startingEdge,
                                                isHorizontal ? children[i].margin().left : children[i].margin().top);
                    } else {
                        const auto margin = isHorizontal
                                                ? Abs(children[i - 1].margin().right - children[i].margin().left)
                                                : Abs(children[i - 1].margin().bottom - children[i].margin().top);
                        component.setConstraint(startingEdge, children[i - 1]->component, endingEdge, margin);
                    }
                }

                // Set ending edge or width / height constraint
                switch (children[i]->type) {
                    using Layout::Internal::ILayout;

                case ILayout::Type::Constant:
                    component.setConstraint(sizeDirection, children[i].as<Layout::Constant>().constant);
                    break;

                case ILayout::Type::Function:
                    component.setConstraint(sizeDirection,
                                            children[i].as<Layout::Function>().func,
                                            children[i].as<Layout::Function>().constant,
                                            children[i].as<Layout::Function>().multiplier);
                    break;

                case ILayout::Type::Percent:
                    component.setConstraint(
                        sizeDirection, parent, sizeDirection, 0.0, children[i].as<Layout::Ratio>().ratio);
                    break;

                case ILayout::Type::Fill:
                    // Fill is only one in the align
                    assert(!inverse);

                    if (i == children.size() - 1) {
                        component.setConstraint(
                            endingEdge,
                            parent,
                            endingEdge,
                            -(isHorizontal ? children[i].margin().right : children[i].margin().bottom));
                    } else if (i == 0) {
                        const auto margin = isHorizontal
                                                ? Abs(children[i + 1].margin().left - children[i].margin().right)
                                                : Abs(children[i + 1].margin().top - children[i].margin().bottom);
                        component.setConstraint(endingEdge, children[i + 1]->component, startingEdge, -margin);
                    } else {
                        const auto margin = isHorizontal
                                                ? Abs(children[i + 1].margin().left - children[i].margin().right)
                                                : Abs(children[i + 1].margin().top - children[i].margin().bottom);
                        component.setConstraint(endingEdge, children[i + 1]->component, startingEdge, -margin);
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

        UIView& Layout(UIView& self, Array<Layout::LayoutProxy>&& children, bool isHorizontal) {
            if (!children.isEmpty()) {
                if (children.size() == 1) {
                    LayoutSingle(self, std::move(children[0]), isHorizontal);
                } else {
                    LayoutMulti(self, std::move(children), isHorizontal);
                }
            }
            return self;
        }
    }

    template <>
    UIView& UIView::layout<Layout::AlignHorizontal>(Array<Layout::LayoutProxy>&& children) {
        return Internal::Layout(*this, std::move(children), true);
    }

    template <>
    UIView& UIView::layout<Layout::AlignVertical>(Array<Layout::LayoutProxy>&& children) {
        return Internal::Layout(*this, std::move(children), false);
    }
}
