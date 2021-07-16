#pragma once

#include "UIRect.h"

namespace s3d::gui {
	class UIImageView : public UIRect {
	public:
		Image image;
		bool manualScalingEnabled = true;

	private:
		DynamicTexture m_texture;
		Rect m_textureRegion;
		double m_scale = 1.0;
		Point m_pixel, m_prePixel;

	public:
		UIImageView() noexcept :
			UIRect()
		{
			backgroundColor = DynamicColor::Background;
		}

		void updateTexture() {
			m_texture.fill(image);
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

		void release() {
			image.release();
			m_texture.release();
		}

		void setImage(const Image& _image);

		void paint(double thickness, const Color& color, bool antialiased = true);

	protected:
		void draw() override;

		void updateLayer(const Rect& scissor) override;

	private:
		double calcInitialScale();
	};
}
