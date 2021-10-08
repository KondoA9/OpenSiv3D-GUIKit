#include <Aoba/Imaging.hpp>

namespace s3d::aoba::Imaging {
	namespace Internal {
		Vec2 RotatePoint(const Vec2& pos, double degrees) {
			const auto angle = degrees * Math::Pi / 180.0;

			return {
				pos.x * cos(angle) - pos.y * sin(angle),
				pos.x * sin(angle) + pos.y * cos(angle)
			};
		}
	}

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

		return (Internal::RotatePoint(Vec2(x, y), -degrees) + Vec2(cx, cy)).asPoint();
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

		return Internal::RotatePoint(Vec2(x, y), degrees) + Vec2(cx, cy);
	}

	Size GetSizeFitsTexture(const Size& textureSize, double degrees) {
		const auto center = Vec2(textureSize.x * 0.5, textureSize.y * 0.5);

		const auto leftTop = Vec2(-center.x, -center.y);
		const auto leftBottom = Vec2(-center.x, -center.y + textureSize.y);
		const auto rightTop = Vec2(-center.x + textureSize.x, -center.y);
		const auto rightBottom = Vec2(-center.x + textureSize.x, -center.y + textureSize.y);

		const auto rotatedLeftTop = Internal::RotatePoint(leftTop, degrees);
		const auto rotatedLeftBottom = Internal::RotatePoint(leftBottom, degrees);
		const auto rotatedRightTop = Internal::RotatePoint(rightTop, degrees);
		const auto rotatedRightBottom = Internal::RotatePoint(rightBottom, degrees);

		const auto left = Min(Min(rotatedLeftTop.x, rotatedRightTop.x), Min(rotatedLeftBottom.x, rotatedRightBottom.x));
		const auto right = Max(Max(rotatedLeftTop.x, rotatedRightTop.x), Max(rotatedLeftBottom.x, rotatedRightBottom.x));
		const auto top = Min(Min(rotatedLeftTop.y, rotatedRightTop.y), Min(rotatedLeftBottom.y, rotatedRightBottom.y));
		const auto bottom = Max(Max(rotatedLeftTop.y, rotatedRightTop.y), Max(rotatedLeftBottom.y, rotatedRightBottom.y));

		return {
			static_cast<Size::value_type>(right - left),
			static_cast<Size::value_type>(bottom - top)
		};
	}
}
