#include "UIZStackedImageView.h"

using namespace s3d::gui;

void UIZStackedImageView::appendImage(const Image& _image, double alphaRate) {
	images.push_back(_image);
	m_textures.push_back(DynamicTexture(_image, TextureDesc::Mipped));
	m_alphas.push_back(255 * alphaRate);
	m_scale = 1.0;
	m_centerPosUpdated = false;
	if (m_textures.size() > 0) {
		m_minScale = calcMinimumScale();
		m_maxScale = calcMaximumScale();
		m_scale = m_minScale;
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

	restrictImageMovement();

	for (size_t i : step(m_textures.size())) {
		m_textures[i].scaled(m_scale).drawAt(m_drawingCenterPos, Color(255, 255, 255, static_cast<uint32>(m_alphas[i])));
	}
}

void UIZStackedImageView::updateLayer() {
	const double preMinScale = m_minScale;
	const double preScale = m_scale;

	UIRect::updateLayer();

	if (m_textures.size() > 0) {
		m_minScale = calcMinimumScale();
		m_maxScale = calcMaximumScale();

		m_scale = preScale * m_minScale / preMinScale;
		if (m_scale < m_minScale) {
			m_scale = m_minScale;
		}
	}

	if (!m_centerPosUpdated) {
		m_drawingCenterPos = m_rect.center();
		m_centerPosUpdated = true;
	}
}

bool UIZStackedImageView::mouseLeftDragging() {
	if (m_textureRegion.leftPressed()) {
		return UIRect::mouseLeftDragging();
	}
	return false;
}

bool UIZStackedImageView::mouseRightDragging() {
	if (m_textureRegion.rightPressed() && UIRect::mouseRightDragging()) {
		const auto movement = Cursor::Pos() - Cursor::PreviousPos();
		m_drawingCenterPos.moveBy(movement);
		return true;
	}
	return false;
}

bool UIZStackedImageView::mouseHovering() {
	if (UIRect::mouseHovering() && m_textures.size() > 0) {
		m_textureRegion = m_textures[0].scaled(m_scale).regionAt(m_drawingCenterPos);
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

bool UIZStackedImageView::mouseWheel() {
	if (UIRect::mouseWheel() && manualScalingEnabled) {
		double k = 0.0;
		if (const int wheel = static_cast<int>(Sign(Mouse::Wheel())); wheel < 0) {
			if (m_scale < m_maxScale) {
				m_scale *= 1.6;
				k = 1.0 - 1.6;
				if (m_scale > m_maxScale) {
					m_scale = m_maxScale;
				}
			}
		}
		else if (wheel > 0) {
			if (m_scale > m_minScale) {
				m_scale *= 0.625;
				k = 1.0 - 0.625;
				if (m_scale < m_minScale) {
					m_scale = m_minScale;
				}
			}
		}

		if (k != 0.0) {
			const auto diff = (m_rect.center() - m_drawingCenterPos) * k;
			m_drawingCenterPos.moveBy(diff);
		}

		return true;
	}
	return false;
}

double UIZStackedImageView::calcMinimumScale() {
	double scale = static_cast<double>(m_rect.w) / static_cast<double>(m_textures[0].width());
	if (const double h = scale * m_textures[0].height(); h > m_rect.h) {
		scale *= m_rect.h / h;
	}
	return scale;
}

double UIZStackedImageView::calcMaximumScale() {
	// Able to zoom in up to 20x20px
	constexpr double limit = 1.0 / 20;
	const double pxh = m_rect.h * limit, pxw = m_rect.w * limit;
	return pxw > pxh ? pxh : pxw;
}

void UIZStackedImageView::restrictImageMovement() {
	const auto center = m_rect.center();

	if (m_textureRegion.w <= m_rect.w) {
		m_drawingCenterPos.x = center.x;
	}
	else {
		if (m_textureRegion.x > m_rect.x) {
			m_drawingCenterPos.x = m_rect.x + m_textureRegion.w * 0.5;
		}
		if (m_textureRegion.x + m_textureRegion.w < m_rect.x + m_rect.w) {
			m_drawingCenterPos.x = m_rect.x + m_rect.w - m_textureRegion.w * 0.5;
		}
	}

	if (m_textureRegion.h <= m_rect.h) {
		m_drawingCenterPos.y = center.y;
	}
	else {
		if (m_textureRegion.y > m_rect.y) {
			m_drawingCenterPos.y = m_rect.y + m_textureRegion.h * 0.5;
		}
		if (m_textureRegion.y + m_textureRegion.h < m_rect.y + m_rect.h) {
			m_drawingCenterPos.y = m_rect.y + m_rect.h - m_textureRegion.h * 0.5;
		}
	}
}