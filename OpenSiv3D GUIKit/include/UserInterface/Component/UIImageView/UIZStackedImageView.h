#pragma once

#include "../UIRect/UIRect.h"

namespace s3d::gui {
	class UIZStackedImageView : public UIRect {
	public:
		bool manualScalingEnabled = true;

	private:
		Array <DynamicTexture> m_textures;
		Array <double> m_alphas;
		Rect m_textureRegion;
		Vec2 m_drawingCenterPos;
		Point m_cursoredPixel, m_preCursoredPixel;
		double m_scale = 1.0, m_minScale = 1.0, m_maxScale = 1.0;

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
				m_textureRegion.x < m_rect.x ? m_rect.x : m_textureRegion.x,
				m_textureRegion.y < m_rect.y ? m_rect.y : m_textureRegion.y,
				m_textureRegion.w < m_rect.w ? m_textureRegion.w : m_rect.w,
				m_textureRegion.h < m_rect.h ? m_textureRegion.h : m_rect.h
			);
		}

		void updateTexture(size_t index, const Image& image) {
			m_textures[index].fillIfNotBusy(image);
		}

		void setScale(double scale) {
			m_scale = scale;
		}

		void setScaleBy(double magnification) {
			m_scale *= magnification;
		}

		void resetScale() {
			m_scale = m_minScale;
		}

		void setAlphaRate(size_t index, double rate) {
			m_alphas[index] = 255 * rate;
		}

		void setViewingCenterPixel(const Point& centerPixel);

		void release();

		void appendImage(const Image& image, double alphaRate = 1.0);

		void removeImage(size_t index);

	protected:
		void draw() override;

		void updateLayer() override;

		bool mouseLeftDragging() override;

		bool mouseRightDragging() override;

		bool mouseHovering() override;

		bool mouseWheel() override;

	private:
		double calcMinimumScale();

		double calcMaximumScale();

		void restrictImageMovement();

		void setDrawingCenterPos(const Vec2& pos);
	};
}