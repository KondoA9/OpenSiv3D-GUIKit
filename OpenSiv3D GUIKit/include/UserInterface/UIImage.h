#pragma once

#include "UIRect.h"

namespace s3d::gui {
	class UIImage : public UIRect {
	public:
		Image image;
		bool manualScalingEnabled = true;

	private:
		DynamicTexture m_texture;
		Rect m_textureRegion;
		double m_scale = 1.0;
		Point m_pixel, m_prePixel;

	public:
		UIImage(const ColorTheme& _backgroundColor = DynamicColor::BackgroundSecondary) :
			UIRect(_backgroundColor)
		{}

		void setImage(const Image& _image) {
			image = _image;
			m_texture = DynamicTexture(image, TextureDesc::Mipped);
			m_scale = 1.0;
			if (m_texture) {
				m_scale = calcInitialScale();
			}
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

		void paint(double thickness, const Color& color, bool antialiased = true) {
			const int32 t = static_cast<int32>(thickness / m_scale);
			Line(m_prePixel, m_pixel).overwrite(image, t > 0 ? t : 1, color, antialiased);
			updateTexture();
		}

		void draw() override {
			UIRect::draw();

			if (m_texture) {
				m_texture.scaled(m_scale).drawAt(m_rect.center());
				m_texture.scaled(m_scale).regionAt(m_rect.center()).drawFrame(2, Palette::Black);
			}
		}

	protected:
		bool mouseHovering() override {
			if (UIRect::mouseHovering()) {
				m_textureRegion = m_texture.scaled(m_scale).regionAt(m_rect.center());
				m_pixel = Point(static_cast<int>((Cursor::Pos().x - m_textureRegion.x) / m_scale), static_cast<int>((Cursor::Pos().y - m_textureRegion.y) / m_scale));
				m_prePixel = Point(static_cast<int>((Cursor::PreviousPos().x - m_textureRegion.x) / m_scale), static_cast<int>((Cursor::PreviousPos().y - m_textureRegion.y) / m_scale));
				if (m_pixel.x < 0) m_pixel.x = 0;
				if (m_pixel.x > m_texture.width()) m_pixel.x = m_texture.width() - 1;
				if (m_pixel.y < 0) m_pixel.y = 0;
				if (m_pixel.y > m_texture.height()) m_pixel.y = m_texture.height() - 1;
				return true;
			}
			return false;
		}

		bool mouseDragging() override {
			if (m_textureRegion.leftPressed()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
				return true;
			}
			return false;
		}

		bool mouseWheel() override {
			if (UIRect::mouseWheel() && manualScalingEnabled) {
				if (const int wheel = static_cast<int>(Sign(Mouse::Wheel())); wheel < 0) {
					m_scale *= 1.6;
				}
				else if (wheel > 0) {
					m_scale *= 0.625;
				}
				return true;
			}
			return false;
		}

	private:
		double calcInitialScale() {
			double scale = static_cast<double>(m_rect.w) / static_cast<double>(m_texture.width());
			if (const double h = scale * m_texture.height(); h > m_rect.h) {
				scale *= m_rect.h / h;
			}
			return scale;
		}
	};
}