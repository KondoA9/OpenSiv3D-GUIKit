#pragma once

#include "../UIRect/UIRect.h"

namespace s3d::gui {
	class UIZStackedImageView : public UIRect {
	public:
		Array<Image> images;
		bool manualScalingEnabled = true;

	private:
		Array <DynamicTexture> m_textures;
		Array <double> m_alphas;
		Rect m_textureRegion;
		double m_scale = 1.0;
		Point m_pixel, m_prePixel;

	public:
		UIZStackedImageView(const ColorTheme& _backgroundColor = DynamicColor::Background) :
			UIRect(_backgroundColor)
		{}

		void updateTexture() {
			for (size_t i : step(m_textures.size())) {
				m_textures[i].fill(images[i]);
			}
		}

		void setScale(double scale) {
			m_scale = scale;
		}

		void setScaleBy(double magnification) {
			m_scale *= magnification;
		}

		void resetScale() {
			m_scale = calcInitialScale();
		}

		void setAlphaRate(size_t index, double rate) {
			m_alphas[index] = 255 * rate;
		}

		void release();

		void appendImage(const Image& _image, double alphaRate = 1.0);

		void paint(size_t index, double thickness, const Color& color, bool antialiased = true);

		void draw() override;

		void updateLayer() override;

	protected:
		bool mouseHovering() override;

		bool mouseDragging() override;

		bool mouseWheel() override;

	private:
		double calcInitialScale();
	};
}