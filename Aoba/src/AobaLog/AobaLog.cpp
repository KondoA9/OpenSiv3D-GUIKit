#include "AobaLog.hpp"

namespace s3d::aoba {
    namespace AobaLog {
        constexpr auto AobaLogger = detail::Logger_impl{};

        void Log(Type type, StringView scope, StringView message) {
            switch (type) {
            case Type::Info:
                AobaLogger << U"Aoba({}) [Info] {}"_fmt(scope, message);
                break;

            case Type::Warning:
                AobaLogger << U"Aoba({}) [Warning] {}"_fmt(scope, message);
                break;

            case Type::Error:
                AobaLogger << U"Aoba({}) [Error] {}"_fmt(scope, message);
                break;

            default:
                assert(false);
            }
        }
    }
}
