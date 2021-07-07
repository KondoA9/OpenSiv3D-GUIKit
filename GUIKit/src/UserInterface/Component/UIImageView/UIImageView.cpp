#include <GUIKit/UIImageView.h>

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
	UIRect::draw();

	if (m_texture) {
		m_texture.scaled(m_scale).drawAt(m_rect.center());
	}
}

void UIImageView::updateLayer(const Rect& scissor) {
	const double preLimit = calcInitialScale();
	const double preScale = m_scale;

	UIRect::updateLayer(scissor);

	const double limit = calcInitialScale();
	m_scale = preScale * limit / preLimit;
	if (m_scale < limit) {
		m_scale = limit;
	}
}

double UIImageView::calcInitialScale() {
	double scale = static_cast<double>(m_rect.w) / static_cast<double>(m_texture.width());
	if (const double h = scale * m_texture.height(); h > m_rect.h) {
		scale *= m_rect.h / h;
	}
	return scale;
}
