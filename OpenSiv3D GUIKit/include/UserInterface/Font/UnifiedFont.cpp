#include "UnifiedFont.h"

namespace s3d::gui::UnifiedFont {
	std::shared_ptr<Font> Small, Medium;

	void Initialize() {
		Small = std::make_shared<Font>(Font(24, Typeface::Light));
		Medium = std::make_shared<Font>(Font(36, Typeface::Light));
	}

	Font& Get(UnifiedFontStyle style) {
		return (style == UnifiedFontStyle::Small) ? *Small
			: *Medium;
	}
}