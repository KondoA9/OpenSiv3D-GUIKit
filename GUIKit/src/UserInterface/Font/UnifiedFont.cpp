#include <GUIKit/UnifiedFont.hpp>
#include <GUIKit/PixelUnit.hpp>

namespace s3d::gui::UnifiedFont {
	namespace Internal {
		size_t counter = 0;

		void RegisterFont(UnifiedFontStyle style, int32 size) {
			const size_t n = static_cast<size_t>(style);

			FontAsset::Register(U"UnifiedFontStyle{}"_fmt(n), size, Typeface::Default, AssetParameter::LoadImmediately());
			FontAsset::Register(U"UnifiedFontStyle{}"_fmt(n + 1), size, Typeface::Light, AssetParameter::LoadImmediately());
			FontAsset::Register(U"UnifiedFontStyle{}"_fmt(n + 2), size, Typeface::Bold, AssetParameter::LoadImmediately());

			counter++;
		}
	}

	void Initialize() {
		Internal::RegisterFont(UnifiedFontStyle::Default, 13_px);
		Internal::RegisterFont(UnifiedFontStyle::Caption, 10_px);
		Internal::RegisterFont(UnifiedFontStyle::Header, 18_px);
	}

	Font Get(UnifiedFontStyle style) {
		return FontAsset(U"UnifiedFontStyle{}"_fmt(static_cast<size_t>(style)));
	}
}
