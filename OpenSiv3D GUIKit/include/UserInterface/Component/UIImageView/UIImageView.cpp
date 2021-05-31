#include "UIImageView.h"

using namespace s3d::gui;

void UIImageView::setImage(const Image& _image) {
	image = _image;
	m_texture = DynamicTexture(image, TextureDesc::Mipped);
	m_scale = 1.0;
	if (m_texture) {
		m_scale = calcInitialScale();
	}
}

void UIImageView::paint(double thickness, const Color& color, bool antialiased) {
	const int32 t = static_cast<int32>(thickness / m_scale);
	Line(m_prePixel, m_pixel).overwrite(image, t > 0 ? t : 1, color, antialiased);
	updateTexture();
}

void UIImageView::draw() {
	if (!drawable()) {
		return;
	}

	UIRect::draw();

	if (m_texture) {
		m_texture.scaled(m_scale).drawAt(m_rect.center());
		m_texture.scaled(m_scale).regionAt(m_rect.center()).drawFrame(2, Palette::Black);
	}
}

void UIImageView::updateLayer() {
	const double preLimit = calcInitialScale();
	const double preScale = m_scale;

	UIRect::updateLayer();

	const double limit = calcInitialScale();
	m_scale = preScale * limit / preLimit;
	if (m_scale < limit) {
		m_scale = limit;
	}
}

bool UIImageView::mouseLeftDragging() {
	if (m_textureRegion.leftPressed()) {
		return UIRect::mouseLeftDragging();
	}
	return false;
}

bool UIImageView::mouseHovering() {
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

bool UIImageView::mouseWheel() {
	if (UIRect::mouseWheel() && manualScalingEnabled) {
		if (const int wheel = static_cast<int>(Sign(Mouse::Wheel())); wheel < 0) {
			// Able to zoom in up to 20x20px
			constexpr double limitation = 1.0 / 20;
			const double pxh = m_rect.h * limitation, pxw = m_rect.w * limitation;
			if (m_texture.height() * pxh > m_textureRegion.h && m_texture.width() * pxw > m_textureRegion.w) {
				m_scale *= 1.6;
			}
		}
		else if (wheel > 0) {
			m_scale *= 0.625;
			if (const double limit = calcInitialScale(); m_scale < limit) {
				m_scale = limit;
			}
		}
		return true;
	}
	return false;
}

double UIImageView::calcInitialScale() {
	double scale = static_cast<double>(m_rect.w) / static_cast<double>(m_texture.width());
	if (const double h = scale * m_texture.height(); h > m_rect.h) {
		scale *= m_rect.h / h;
	}
	return scale;
}