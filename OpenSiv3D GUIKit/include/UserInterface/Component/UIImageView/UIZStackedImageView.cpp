#include "UIZStackedImageView.h"

using namespace s3d::gui;

void UIZStackedImageView::appendImage(const Image& _image, double alphaRate) {
	images.push_back(_image);
	m_textures.push_back(DynamicTexture(_image, TextureDesc::Mipped));
	m_alphas.push_back(255 * alphaRate);
	m_scale = 1.0;
	if (m_textures.size() > 0) {
		m_scale = calcInitialScale();
	}
}

void UIZStackedImageView::paint(size_t index, double thickness, const Color& color, bool antialiased) {
	const int32 t = static_cast<int32>(thickness / m_scale);
	Line(m_prePixel, m_pixel).overwrite(images[index], t > 0 ? t : 1, color, antialiased);
	updateTexture();
}

void UIZStackedImageView::release() {
	images.release();
	m_textures.release();
	m_alphas.release();
}

void UIZStackedImageView::draw() {
	if (!drawable()) {
		return;
	}

	UIRect::draw();

	for (size_t i : step(m_textures.size())) {
		m_textures[i].scaled(m_scale).drawAt(m_rect.center(), Color(255, 255, 255, static_cast<uint32>(m_alphas[i])));
		m_textures[i].scaled(m_scale).regionAt(m_rect.center()).drawFrame(2, Palette::Black);
	}
}

void UIZStackedImageView::updateLayer() {
	const double preLimit = calcInitialScale();
	const double preScale = m_scale;

	UIRect::updateLayer();

	const double limit = calcInitialScale();
	m_scale = preScale * limit / preLimit;
	if (m_scale < limit) {
		m_scale = limit;
	}
}

bool UIZStackedImageView::mouseHovering() {
	if (UIRect::mouseHovering() && m_textures.size() > 0) {
		m_textureRegion = m_textures[0].scaled(m_scale).regionAt(m_rect.center());
		m_pixel = Point(static_cast<int>((Cursor::Pos().x - m_textureRegion.x) / m_scale), static_cast<int>((Cursor::Pos().y - m_textureRegion.y) / m_scale));
		m_prePixel = Point(static_cast<int>((Cursor::PreviousPos().x - m_textureRegion.x) / m_scale), static_cast<int>((Cursor::PreviousPos().y - m_textureRegion.y) / m_scale));
		if (m_pixel.x < 0) m_pixel.x = 0;
		if (m_pixel.x > m_textures[0].width()) m_pixel.x = m_textures[0].width() - 1;
		if (m_pixel.y < 0) m_pixel.y = 0;
		if (m_pixel.y > m_textures[0].height()) m_pixel.y = m_textures[0].height() - 1;
		return true;
	}
	return false;
}

bool UIZStackedImageView::mouseDragging() {
	if (m_textureRegion.leftPressed()) {
		return UIRect::mouseDragging();
	}
	return false;
}

bool UIZStackedImageView::mouseWheel() {
	if (UIRect::mouseWheel() && manualScalingEnabled) {
		if (const int wheel = static_cast<int>(Sign(Mouse::Wheel())); wheel < 0) {
			// Able to zoom in up to 20x20px
			constexpr double limitation = 1.0 / 20;
			const double pxh = m_rect.h * limitation, pxw = m_rect.w * limitation;
			if (m_textures[0].height() * pxh > m_textureRegion.h && m_textures[0].width() * pxw > m_textureRegion.w) {
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

double UIZStackedImageView::calcInitialScale() {
	double scale = static_cast<double>(m_rect.w) / static_cast<double>(m_textures[0].width());
	if (const double h = scale * m_textures[0].height(); h > m_rect.h) {
		scale *= m_rect.h / h;
	}
	return scale;
}