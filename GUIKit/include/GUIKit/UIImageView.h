#pragma once

#include "UIRect.h"

namespace s3d::gui {
	class UIImageView : public UIRect {
	public:
		bool manualScalingEnabled = true;

	private:
		// Textures
		Array <DynamicTexture> m_textures;
		Array <double> m_alphas;
		RectF m_textureRegion;

		// Position
		Vec2 m_drawingCenterPos;
		Point m_cursoredPixel, m_preCursoredPixel;

		// Scaling
		const uint32 m_minPixel = 50;
		uint32 m_maxPixel = 0;
		double m_scale = 1.0, m_minScale = 1.0, m_maxScale = 1.0, m_scaleRate = 0.0;

		// Rotation
		RectF m_rotatedTextureRegion;
		Vec2 m_baseRotatedTextureSize, m_rotatedTextureSize;
		double m_angle = 0.0;

	public:
		explicit UIImageView() noexcept :
			UIRect()
		{}

		const Point& currentPixel() const {
			return m_cursoredPixel;
		}

		const Point& prePixel() const {
			return m_preCursoredPixel;
		}

		const RectF& textureRegion() const {
			return m_textureRegion;
		}

		const RectF& rotatedTextureRegion() const {
			return m_rotatedTextureRegion;
		}

		double scale() const {
			return m_scale;
		}

		double minimumScale() const {
			return m_minScale;
		}

		double maximumScale() const {
			return m_maxScale;
		}

		double scaleRate() const {
			return m_scaleRate;
		}

		/// <returns>degrees</returns>
		double angle() const {
			return m_angle * 180.0 / Math::Pi;
		}

		size_t texturesCount() const {
			return m_textures.size();
		}

		RectF visibleTextureRect() const {
			return RectF(
				m_rotatedTextureRegion.x < rect().x ? rect().x : m_rotatedTextureRegion.x,
				m_rotatedTextureRegion.y < rect().y ? rect().y : m_rotatedTextureRegion.y,
				m_rotatedTextureRegion.w < rect().w ? m_rotatedTextureRegion.w : rect().w,
				m_rotatedTextureRegion.h < rect().h ? m_rotatedTextureRegion.h : rect().h
			);
		}

		void release() override;

		void releaseImages();

		void rotate(double degrees);

		void updateTexture(size_t index, const Image& image) {
			m_textures[index].fill(image);
		}

		void updateTexture(size_t index, const Image& image, const Rect& rect) {
			m_textures[index].fillRegion(image, rect);
		}

		void updateTextureIfNotBusy(size_t index, const Image& image) {
			m_textures[index].fillIfNotBusy(image);
		}

		void updateTextureIfNotBusy(size_t index, const Image& image, const Rect& rect) {
			m_textures[index].fillRegionIfNotBusy(image, rect);
		}

		void setAlphaRate(size_t index, double rate) {
			m_alphas[index] = 255 * rate;
		}

		/// <param name="rate">From 0.0 to 1.0</param>
		void setScale(double rate);

		void resetScale();

		void setViewingCenterPixel(const Point& centerPixel);

		void appendImage(const Image& image, double alphaRate = 1.0);

		void removeImage(size_t index);

	protected:
		void initialize() override;

		void draw() override;

		void updateLayer(const Rect& scissor) override;

	private:
		double calcMinimumScale();

		double calcMaximumScale();

		void restrictImageMovement();

		void updateTextureRegion();

		void setDrawingCenterPos(const Vec2& pos);
	};
}
