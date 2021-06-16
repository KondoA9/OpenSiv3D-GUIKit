#include <GUIKit/UnifiedFont.h>
#include <GUIKit/PixelUnit.h>

namespace s3d::gui::UnifiedFont {
	std::shared_ptr<Font> Small, Medium, Large;

	void Initialize() {
		Small = std::make_shared<Font>(Font(16_px, Typeface::Light));
		Medium = std::make_shared<Font>(Font(24_px, Typeface::Light));
		Large = std::make_shared<Font>(Font(32_px, Typeface::Light));
	}

	Font& Get(UnifiedFontStyle style) {
		switch (style)
		{
		case s3d::gui::UnifiedFontStyle::Small:
			return *Small;
			break;
		case s3d::gui::UnifiedFontStyle::Medium:
			return *Medium;
			break;
		case s3d::gui::UnifiedFontStyle::Large:
			return *Large;
			break;
		}
		return *Medium;
	}
}