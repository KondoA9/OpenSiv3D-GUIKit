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
		double m_scale = 1.0, m_minScale = 1.0, m_maxScale = 1.0;
		Point m_pixel, m_prePixel;
		bool m_centerPosUpdated = false;
		Vec2 m_drawingCenterPos;

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
			return m_pixel;
		}

		Point prePixel() const {
			return m_prePixel;
		}

		size_t texturesCount() const {
			return m_textures.size();
		}

		void updateTexture(size_t index, const Image& image) {
			if (m_textures.size() > index) {
				m_textures[index].fillIfNotBusy(image);
			}
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

		void release();

		void appendImage(const Image& image, double alphaRate = 1.0);

		void draw() override;

		void updateLayer() override;

	protected:
		bool mouseLeftDragging() override;

		bool mouseRightDragging() override;

		bool mouseHovering() override;

		bool mouseWheel() override;

	private:
		double calcMinimumScale();

		double calcMaximumScale();

		void restrictImageMovement();
	};
}