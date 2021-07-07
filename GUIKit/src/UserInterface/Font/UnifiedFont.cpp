#include <GUIKit/UnifiedFont.h>
#include <GUIKit/PixelUnit.h>

namespace s3d::gui::UnifiedFont {
	Array<std::shared_ptr<Font>> Fonts;

	void Initialize() {
		Fonts = {
			std::make_shared<Font>(Font(16_px, Typeface::Regular)),
			std::make_shared<Font>(Font(16_px, Typeface::Light)),
			std::make_shared<Font>(Font(16_px, Typeface::Bold)),

			std::make_shared<Font>(Font(24_px, Typeface::Regular)),
			std::make_shared<Font>(Font(24_px, Typeface::Light)),
			std::make_shared<Font>(Font(24_px, Typeface::Bold)),

			std::make_shared<Font>(Font(32_px, Typeface::Regular)),
			std::make_shared<Font>(Font(32_px, Typeface::Light)),
			std::make_shared<Font>(Font(32_px, Typeface::Bold))
		};
	}

	const Font& Get(UnifiedFontStyle style) {
		const size_t index = static_cast<size_t>(style);
		assert(index < Fonts.size());
		return *Fonts[index].get();
	}
}
