#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba {
	enum class AobaLogType {
		Info,
		Warning,
		Error
	};

	// Output is "Aoba(scope) [Info / Warning / Error] message"
	void AobaLog(AobaLogType type, const String& scope, const String& message) {
		switch (type)
		{
		case AobaLogType::Info:
			Logger << U"Aoba({}) [Info] {}"_fmt(scope, message);
			break;

		case AobaLogType::Warning:
			Logger << U"Aoba({}) [Warning] {}"_fmt(scope, message);
			break;

		case AobaLogType::Error:
			Logger << U"Aoba({}) [Error] {}"_fmt(scope, message);
			break;

		default:
			assert(false);
		}
	}
}