#pragma once

#include "UIRect.h"

namespace s3d::gui {
	class UIZStackedImageView : public UIRect {
	public:
		bool manualScalingEnabled = true;

	private:
		// Textures
		Array <DynamicTexture> m_textures;
		Array <double> m_alphas;
		Rect m_textureRegion;

		// Position
		Vec2 m_drawingCenterPos;
		Point m_cursoredPixel, m_preCursoredPixel;

		// Scaling
		double m_scale = 1.0, m_minScale = 1.0, m_maxScale = 1.0, m_scaleRate = 0.0;
		const uint32 m_minPixel = 50;
		uint32 m_maxPixel = 0;

	public:
		UIZStackedImageView(const ColorTheme& _backgroundColor = DynamicColor::Background) :
			UIRect(_backgroundColor)
		{}

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

		Point currentPixel() const {
			return m_cursoredPixel;
		}

		Point prePixel() const {
			return m_preCursoredPixel;
		}

		size_t texturesCount() const {
			return m_textures.size();
		}

		Rect textureRegion() const {
			return m_textureRegion;
		}

		Rect visibleTextureRect() const {
			return Rect(
				m_textureRegion.x < static_cast<int>(m_rect.x) ? static_cast<int>(m_rect.x) : m_textureRegion.x,
				m_textureRegion.y < static_cast<int>(m_rect.y) ? static_cast<int>(m_rect.y) : m_textureRegion.y,
				m_textureRegion.w < static_cast<int>(m_rect.w) ? m_textureRegion.w : static_cast<int>(m_rect.w),
				m_textureRegion.h < static_cast<int>(m_rect.h) ? m_textureRegion.h : static_cast<int>(m_rect.h)
			);
		}

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

		void setScaleBy(double magnification) {
			m_scale *= magnification;
		}

		void setAlphaRate(size_t index, double rate) {
			m_alphas[index] = 255 * rate;
		}

		// Set scale by rate from 0.0 to 1.0
		void setScale(double rate);

		void resetScale();

		void setViewingCenterPixel(const Point& centerPixel);

		void release();

		void appendImage(const Image& image, double alphaRate = 1.0);

		void removeImage(size_t index);

	protected:
		void initialize() override;

		void draw(const Rect& scissor) override;

		void updateLayer() override;

	private:
		double calcMinimumScale();

		double calcMaximumScale();

		void restrictImageMovement();

		void setDrawingCenterPos(const Vec2& pos);
	};
}
