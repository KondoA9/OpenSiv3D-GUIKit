#include <GUIKit/Imaging.h>

namespace s3d::gui::Imaging {
	Point ScenePosToPixel(const Vec2& pos, const RectF& textureRegion, double scale) {
		return Point(static_cast<uint32>((pos.x - textureRegion.x) / scale), static_cast<uint32>((pos.y - textureRegion.y) / scale));
	}

	Vec2 PixelToScenePos(const Point& pixel, const RectF& textureRegion, double scale) {
		return Vec2(pixel.x * scale + textureRegion.x, pixel.y * scale + textureRegion.y);
	}
}