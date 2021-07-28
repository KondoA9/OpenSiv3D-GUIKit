#include <GUIKit/Imaging.h>

namespace s3d::gui::Imaging {
	Point ScenePosToPixel(const Vec2& pos, const RectF& textureRegion, double scale, double degrees) {
		// Transform cursor pos to pixel without rotation
		const Point pixel = Vec2((pos.x - textureRegion.x) / scale, (pos.y - textureRegion.y) / scale).asPoint();

		// Size
		const auto width = textureRegion.w / scale;
		const auto height = textureRegion.h / scale;

		// Center of the texture
		const auto cx = width * 0.5;
		const auto cy = height * 0.5;

		// Fix the origin to center of the texture
		const auto x = pixel.x - cx;
		const auto y = pixel.y - cy;

		// Fix coordinate
		const auto angle = -degrees * Math::Pi / 180.0;

		// Rotate
		const auto tx = x * cos(angle) - y * sin(angle) + cx;
		const auto ty = x * sin(angle) + y * cos(angle) + cy;

		return Vec2(tx, ty).asPoint();
	}

	Vec2 PixelToScenePos(const Point& pixel, const RectF& textureRegion, double scale, double degrees) {
		// Transform pixel to cursor pos without rotation
		const Vec2 pos = Vec2(pixel.x * scale + textureRegion.x, pixel.y * scale + textureRegion.y);

		// Center of the texture
		const auto cx = textureRegion.center().x;
		const auto cy = textureRegion.center().y;

		// Fix the origin to center of the texture
		const auto x = pos.x - cx;
		const auto y = pos.y - cy;

		const auto angle = degrees * Math::Pi / 180.0;

		// Rotate
		const auto tx = x * cos(angle) - y * sin(angle) + cx;
		const auto ty = x * sin(angle) + y * cos(angle) + cy;

		return Vec2(tx, ty);
	}
}
