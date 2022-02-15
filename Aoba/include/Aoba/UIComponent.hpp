#pragma once

#include <Siv3D.hpp>

#include "DynamicColor.hpp"
#include "InputEventHandler.hpp"
#include "Layer.hpp"
#include "MouseEvent.hpp"

// Create InputEvent in namespace s3d::aoba::Event::Component
#define AobaCreateEventComponent(EVENT_NAME) AobaCreateEventNSEvent(Component, EVENT_NAME)

// Create InputEvent in namespace s3d::aoba::Event::Component::NAMESPACE
#define AobaCreateEventComponentNS(NAMESPACE, EVENT_NAME) AobaCreateEventNSEvent(Component::NAMESPACE, EVENT_NAME)

AobaCreateEventComponent(Focused);
AobaCreateEventComponent(UnFocused);

namespace s3d::aoba {
    class Core;
    class Factory;
    class InputEventManager;
    class PageManager;
    class UIView;

    class UIComponent {
        struct MouseClickCondition {
            bool down = false, up = false, press = false;

            constexpr const MouseClickCondition& operator&=(bool condition) noexcept {
                down &= condition;
                up &= condition;
                press &= condition;
                return *this;
            }
        };

        struct MouseCondition {
            MouseClickCondition left, right;
            bool hover = false, preHover = false;

            constexpr const MouseCondition& operator&=(bool condition) noexcept {
                left &= condition;
                right &= condition;
                hover &= condition;
                return *this;
            }
        };

        friend Core;
        friend Factory;
        friend InputEventManager;
        friend PageManager;
        friend UIView;

    public:
        ColorTheme backgroundColor, frameColor;
        String tooltipMessage = U"";
        double frameThickness = 1.0;
        bool fillInner = true, drawFrame = false;
        bool penetrateMouseEvent = false;
        bool hidden = false, exist = true, controllable = true;

    private:
        static Optional<size_t> m_FocusedComponentId, m_PreviousFocusedComponentId;

        const size_t m_id;

        bool m_initializedColors = false;

        Layer m_layer;
        Array<Layer*> m_dependentLayers;
        Rect m_drawableRegion    = Rect();
        bool m_needToUpdateLayer = true;

        // Mouse event
        MouseCondition m_mouseCondition;
        double m_clickIntervalTimer = 0.0;  // If mouse released within 0.5s, mouseDown event will be called
        bool m_mouseDownEnable = false, m_mouseDownRaw = false;
        bool m_mouseDragging = false;
        Vec2 m_clickedPos    = Vec2();
        Array<InputEventHandler> m_inputEventHandlers;

    public:
        UIComponent() = delete;

        UIComponent(const UIComponent&) = delete;

        UIComponent& operator=(const UIComponent&) = delete;

        virtual ~UIComponent();

        virtual void release() {}

        void setConstraint(LayerDirection direction,
                           UIComponent& component,
                           LayerDirection toDirection,
                           double constant   = 0.0,
                           double multiplier = 1.0);

        void setConstraint(LayerDirection direction, double constant = 0.0, double multiplier = 1.0);

        void setConstraint(LayerDirection direction,
                           const std::function<double()>& func,
                           double constant   = 0.0,
                           double multiplier = 1.0);

        void removeConstraint(LayerDirection direction);

        void removeAllConstraints();

        const Layer& layer() const {
            return m_layer;
        }

        size_t id() const {
            return m_id;
        }

        bool isFocused() const {
            return m_FocusedComponentId && m_FocusedComponentId == m_id;
        }

        bool updatable() const {
            return exist;
        }

        bool layerUpdatable() const {
            return updatable();
        }

        bool drawable() const {
            const bool insideRegion =
                m_layer.top() <= static_cast<double>(m_drawableRegion.y) + static_cast<double>(m_drawableRegion.h)
                && m_layer.left() <= static_cast<double>(m_drawableRegion.x) + static_cast<double>(m_drawableRegion.w)
                && m_layer.bottom() >= m_drawableRegion.y && m_layer.right() >= m_drawableRegion.x;

            return updatable() && !hidden && insideRegion;
        }

        bool eventUpdatable() const {
            return drawable() && controllable;
        }

        void requestToUpdateLayer() {
            m_needToUpdateLayer = true;
        }

        void focus();

        void unFocus();

        template <class T>
        void addEventListener(const std::function<void(const T&)>& f, bool primary = false) {
            const auto handler = InputEventHandler::Create<T>(([f](InputEvent e) { f(*static_cast<T*>(&e)); }));

            if (primary) {
                m_inputEventHandlers.push_front(handler);
            } else {
                m_inputEventHandlers.push_back(handler);
            }
        }

        template <class T>
        void addEventListener(const std::function<void()>& f, bool primary = false) {
            addEventListener<T>([f](const InputEvent&) { f(); }, primary);
        }

    protected:
        // Called once at constructed.
        // If you need to call addEventlistener etc to implement the default behavior, define this function.
        // Do not forget to call super::initialize().
        virtual void initialize() {}

        // Called once before draw().
        // This function is intended to initialize background color. (ex. backgroundColor = defaultColor; in UIButton)
        // Override and implement this function if the component has default colors.
        virtual void initializeColors() {}

        virtual void update() {}

        virtual void updateLayer(const Rect& scissor);

        virtual void updateMouseIntersection() = 0;

        virtual void updateInputEvents();

        virtual void draw() const = 0;

        void registerInputEvent(const InputEvent& e);

        const MouseCondition& mouseCondition() const {
            return m_mouseCondition;
        }

        // Do not call this function if the component is not UIView
        virtual void _destroy();

        // Do not call this function if the component is not UIRect or UICircle
        void _updateMouseCondition(
            bool leftDown, bool leftUp, bool leftPress, bool rightDown, bool rightUp, bool rightPress, bool hover);

    private:
        UIComponent(size_t id) noexcept;

        static void UpdateFocusEvent();

        virtual bool updateLayerIfNeeded(const Rect& scissor);
    };
}
