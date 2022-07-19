#pragma once

#include <functional>

namespace s3d::aoba {
    namespace Internal {
        template <typename T>
        class Prop final {
        private:
            T m_value                             = T();
            std::function<void(const T&)> m_onSet = nullptr;

        public:
            Prop() = default;

            Prop(const T& value) noexcept : m_value(value) {}

            Prop(const T& value, const std::function<void(const T&)>& onSet) noexcept :
                m_value(value), m_onSet(onSet) {}

            const T& value() const noexcept {
                return m_value;
            }

            void set(const T& _value) {
                m_value = _value;

                if (m_onSet) {
                    m_onSet(_value);
                }
            }

            operator const T&() const noexcept {
                return value();
            }

            Prop& operator=(const T& _value) {
                set(_value);
                return *this;
            }
        };
    }
}
