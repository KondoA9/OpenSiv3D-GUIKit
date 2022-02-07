#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba {
	namespace AobaLog {
		enum class Type {
			Info,
			Warning,
			Error
		};

		// Output is "Aoba(scope) [Info / Warning / Error] message"
		void Log(Type type, const String& scope, const String& message);
	}
}