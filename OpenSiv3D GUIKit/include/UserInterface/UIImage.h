#pragma once

#include "UIRect.h"

namespace s3d::gui {
	class UIImage : public UIRect {
	public:
		Image image;

	private:
		DynamicTexture m_texture;
		Rect m_textureRegion;
		double m_scale = 1.0;
		Point m_pixel, m_prePixel;

	public:
		UIImage(const ColorTheme& _backgroundColor = DynamicColor::backgroundSecondary) :
			UIRect(_backgroundColor) 
		{}

		void setImage(const Image& _image) {
			image = _image;
			m_texture = DynamicTexture(image, TextureDesc::Mipped);
			shouldUpdateLayer = true;
		}

		void updateTexture() {
			m_texture.fill(image);
		}

		void scale(double scale) {
			m_scale = scale;
		}

		void scaleBy(double magnification) {
			m_scale *= magnification;
		}

		void updateShape() override {
			UIRect::updateShape();
			if (m_texture) {
				m_scale = 1.0;
				if (m_texture.width() > rect.w) {
					m_scale = static_cast<double>(rect.w) / static_cast<double>(m_texture.width());
				}
				if (const double h = m_scale * m_texture.height(); h > rect.h) {
					m_scale *= rect.h / h;
				}
			}
		}

		void draw() override {
			UIRect::draw();
			if (m_texture) {
				m_texture.scaled(m_scale).drawAt(rect.center());
				m_texture.scaled(m_scale).regionAt(rect.center()).drawFrame(2, Palette::Black);
			}

			
			static int index = 1;
			const int scroll = Sign(Mouse::Wheel());
			if (scroll < 0 || scroll>0 && index > 1) {
				index += -scroll;
			}
			m_scale = Pow(2, index);
			/*if (const double scale = m_scale + Math::Sign(-Mouse::Wheel()); scale > 0.0) {
				m_scale = scale;
			}*/
		}

		void paint(double thickness, const Color& color, bool antialiased = true) {
			Line(m_prePixel, m_pixel).overwrite(image, static_cast<int32>(thickness), color, antialiased);
			updateTexture();
		}

	protected:
		bool hovering() override {
			if (UIRect::hovering()) {
				m_textureRegion = m_texture.scaled(m_scale).regionAt(rect.center());
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

		bool dragging() override {
			if (m_textureRegion.leftPressed()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
				return true;
			}
			return false;
		}
	};
}