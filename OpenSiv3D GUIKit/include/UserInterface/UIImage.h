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
		UIImage(const ColorTheme& _backgroundColor = DynamicColor::BackgroundSecondary) :
			UIRect(_backgroundColor)
		{}

		void setImage(const Image& _image) {
			image = _image;
			m_texture = DynamicTexture(image, TextureDesc::Mipped);
			m_scale = 1.0;
			if (m_texture) {
				if (m_texture.width() > m_rect.w) {
					m_scale = static_cast<double>(m_rect.w) / static_cast<double>(m_texture.width());
				}
				if (const double h = m_scale * m_texture.height(); h > m_rect.h) {
					m_scale *= m_rect.h / h;
				}
			}
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

		void draw() override {
			UIRect::draw();

			if (m_texture) {
				m_texture.scaled(m_scale).drawAt(m_rect.center());
				m_texture.scaled(m_scale).regionAt(m_rect.center()).drawFrame(2, Palette::Black);
			}
		}

		void paint(double thickness, const Color& color, bool antialiased = true) {
			Line(m_prePixel, m_pixel).overwrite(image, static_cast<int32>(thickness), color, antialiased);
			updateTexture();
		}

	protected:
		bool hovering() override {
			if (UIRect::hovering()) {
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

		bool dragging() override {
			if (m_textureRegion.leftPressed()) {
				callMouseEventHandler(MouseEvent(MouseEventType::Dragging, this));
				return true;
			}
			return false;
		}

		void updateMouseEvent() override {
			UIRect::updateMouseEvent();

			if (const int scroll = static_cast<int>(Sign(Mouse::Wheel())); scroll < 0) {
				m_scale *= 1.6;
			}
			else if (scroll > 0) {
				m_scale *= 0.625;
			}
		}
	};
}