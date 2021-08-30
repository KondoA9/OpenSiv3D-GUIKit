#include <GUIKit/UnifiedFont.hpp>
#include <GUIKit/PixelUnit.hpp>

namespace s3d::gui::UnifiedFont {
	Array<std::shared_ptr<Font>> Fonts;

	void Initialize() {
		const int32 fontsizeSmall = 12_px;
		const int32 fontsizeMedium = 18_px;
		const int32 fontsizeLarge = 26_px;

		Fonts = {
			std::make_shared<Font>(Font(fontsizeSmall, Typeface::Regular)),
			std::make_shared<Font>(Font(fontsizeSmall, Typeface::Light)),
			std::make_shared<Font>(Font(fontsizeSmall, Typeface::Bold)),

			std::make_shared<Font>(Font(fontsizeMedium, Typeface::Regular)),
			std::make_shared<Font>(Font(fontsizeMedium, Typeface::Light)),
			std::make_shared<Font>(Font(fontsizeMedium, Typeface::Bold)),

			std::make_shared<Font>(Font(fontsizeLarge, Typeface::Regular)),
			std::make_shared<Font>(Font(fontsizeLarge, Typeface::Light)),
			std::make_shared<Font>(Font(fontsizeLarge, Typeface::Bold))
		};
	}

	const Font& Get(UnifiedFontStyle style) {
		const size_t index = static_cast<size_t>(style);
		assert(index < Fonts.size());
		return *Fonts[index].get();
	}
}
