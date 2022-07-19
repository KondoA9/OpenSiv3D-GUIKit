#pragma once

#include <Siv3D.hpp>

// Create InputEvent
#define AobaCreateEvent(EVENT_NAME)                                                                                  \
    struct EVENT_NAME : public s3d::aoba::InputEvent {                                                               \
        explicit EVENT_NAME(s3d::aoba::UIComponent* _component, bool callIfComponentInFront = true) noexcept :       \
            s3d::aoba::InputEvent(typeid(EVENT_NAME).hash_code(), *_component, callIfComponentInFront) {}            \
        explicit EVENT_NAME(const s3d::aoba::UIComponent& _component, bool callIfComponentInFront = true) noexcept : \
            s3d::aoba::InputEvent(typeid(EVENT_NAME).hash_code(), _component, callIfComponentInFront) {}             \
    };

// Create InputEvent in namespace NAMESPACE
#define AobaCreateEventNS(NAMESPACE, EVENT_NAME) \
    namespace NAMESPACE {                        \
        AobaCreateEvent(EVENT_NAME)              \
    }

// Create InputEvent in namespace s3d::aoba::Event::NAMESPACE
#define AobaCreateEventNSEvent(NAMESPACE, EVENT_NAME) AobaCreateEventNS(s3d::aoba::Event::NAMESPACE, EVENT_NAME)

namespace s3d::aoba {
    class UIComponent;

    struct InputEvent {
        const size_t id, componentId;
        const double wheel;
        const Vec2 pos, previousPos;
        const bool callIfComponentInFront;

        InputEvent() = delete;

        InputEvent(size_t _id, const UIComponent& _component, bool _callIfComponentInFront) noexcept;

        InputEvent(const InputEvent&) noexcept = default;

        InputEvent(InputEvent&&) noexcept = default;

        InputEvent& operator=(const InputEvent&) = delete;

        InputEvent& operator=(InputEvent&&) noexcept {
            return *this;
        }

        virtual ~InputEvent() noexcept {};
    };
}
