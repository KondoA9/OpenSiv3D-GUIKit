#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	enum class UnifiedFontStyle : size_t {
		Default,
		DefaultLight,
		DefaultBold,

		Caption,
		CaptionLight,
		CaptionBold,

		Header,
		HeaderLight,
		HeaderBold
	};

	namespace UnifiedFont {
		void Initialize();

		Font Get(UnifiedFontStyle style = UnifiedFontStyle::Default);
	}
}
