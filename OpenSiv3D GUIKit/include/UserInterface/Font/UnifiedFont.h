#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	enum class UnifiedFontStyle {
		Small,
		Medium,
		Large
	};

	namespace UnifiedFont {
		void Initialize();

		Font& Get(UnifiedFontStyle style);
	}
}