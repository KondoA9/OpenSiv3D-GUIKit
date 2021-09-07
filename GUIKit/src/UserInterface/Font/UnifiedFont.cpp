#include <GUIKit/UnifiedFont.hpp>
#include <GUIKit/PixelUnit.hpp>

namespace s3d::gui::UnifiedFont {
	Array<std::shared_ptr<Font>> Fonts;
	HashTable<String, std::shared_ptr<Font>> UserFonts;

	void Initialize() {
		const int32 fontDefault = 13_px;
		const int32 fontCaption = 10_px;
		const int32 fontHeader = 18_px;

		Fonts = {
			std::make_shared<Font>(Font(fontDefault, Typeface::Regular)),
			std::make_shared<Font>(Font(fontDefault, Typeface::Light)),
			std::make_shared<Font>(Font(fontDefault, Typeface::Bold)),

			std::make_shared<Font>(Font(fontCaption, Typeface::Regular)),
			std::make_shared<Font>(Font(fontCaption, Typeface::Light)),
			std::make_shared<Font>(Font(fontCaption, Typeface::Bold)),

			std::make_shared<Font>(Font(fontHeader, Typeface::Regular)),
			std::make_shared<Font>(Font(fontHeader, Typeface::Light)),
			std::make_shared<Font>(Font(fontHeader, Typeface::Bold))
		};
	}

	void Register(const String& identifier, const Font& font) {
		if (!UserFonts.contains(identifier)) {
			UserFonts.emplace(identifier, std::make_shared<Font>(font));
		}
	}

	void Register(const String& identifier, int32 size) {
		Register(identifier, Font(size));
	}

	const Font& Get(const String& identifier) {
		const auto& font = *UserFonts[identifier].get();
		assert(!font.isEmpty());
		return font;
	}

	const Font& Get(UnifiedFontStyle style) {
		const size_t index = static_cast<size_t>(style);
		assert(index < Fonts.size());
		return *Fonts[index].get();
	}
}
