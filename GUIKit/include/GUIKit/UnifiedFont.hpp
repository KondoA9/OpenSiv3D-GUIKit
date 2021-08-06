#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	enum class UnifiedFontStyle :size_t {
		Small,
		SmallLight,
		SmallBold,

		Medium,
		MediumLight,
		MediumBold,

		Large,
		LargeLight,
		LargeBold
	};

	namespace UnifiedFont {
		void Initialize();

		const Font& Get(UnifiedFontStyle style);
	}
}
