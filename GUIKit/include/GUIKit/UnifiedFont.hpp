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

		void Register(const String& identifier, const Font& font);

		void Register(const String& identifier, int32 size);

		const Font& Get(UnifiedFontStyle style = UnifiedFontStyle::Default);

		const Font& Get(const String& identifier);
	}
}
