#include "UIZStackedImageView.h"
#include "../../../Imaging/Imaging.h"

using namespace s3d::gui;

void UIZStackedImageView::appendImage(const Image& image, double alphaRate) {
	m_textures.push_back(DynamicTexture(image, TextureDesc::Mipped));
	m_alphas.push_back(255 * alphaRate);

	m_minScale = calcMinimumScale();
	m_maxScale = calcMaximumScale();
	m_scale = m_minScale;

	setDrawingCenterPos(m_rect.center());

	requestToUpdateLayer();
}

void UIZStackedImageView::removeImage(size_t index) {
	m_textures.remove_at(index);
	m_alphas.remove_at(index);
}

void UIZStackedImageView::release() {
	m_textures.release();
	m_alphas.release();
	setDrawingCenterPos(m_rect.center());
}

void UIZStackedImageView::draw() {
	UIRect::draw();

	if (m_textures) {
		m_textureRegion = m_textures[0].scaled(m_scale).regionAt(m_drawingCenterPos);
	}

	restrictImageMovement();

	for (size_t i : step(m_textures.size())) {
		m_textures[i].scaled(m_scale).drawAt(m_drawingCenterPos, Color(255, 255, 255, static_cast<uint32>(m_alphas[i])));
	}
}

void UIZStackedImageView::updateLayer() {
	const double preMinScale = m_minScale;
	const double preScale = m_scale;

	UIRect::updateLayer();

	if (m_textures) {
		m_minScale = calcMinimumScale();
		m_maxScale = calcMaximumScale();

		m_scale = preScale * m_minScale / preMinScale;
		m_scale = Clamp(m_scale, m_minScale, m_maxScale);
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
		setDrawingCenterPos(m_drawingCenterPos.movedBy(movement));
		return true;
	}
	return false;
}

bool UIZStackedImageView::mouseHovering() {
	if (UIRect::mouseHovering() && m_textures) {
		m_cursoredPixel = Imaging::ScenePosToPixel(Cursor::Pos(), m_textureRegion, m_scale);
		m_preCursoredPixel = Imaging::ScenePosToPixel(Cursor::PreviousPos(), m_textureRegion, m_scale);
		m_cursoredPixel.x = Clamp(m_cursoredPixel.x, 0, m_textures[0].width() - 1);
		m_cursoredPixel.y = Clamp(m_cursoredPixel.y, 0, m_textures[0].height() - 1);
		return true;
	}
	return false;
}

bool UIZStackedImageView::mouseWheel() {
	if (UIRect::mouseWheel() && manualScalingEnabled) {
		const double scalingRate = static_cast<int>(Sign(Mouse::Wheel())) < 0 ? 1.6 : 0.625;
		const double k = 1.0 - scalingRate;

		m_scale *= scalingRate;
		m_scale = Clamp(m_scale, m_minScale, m_maxScale);

		const auto diff = (m_rect.center() - m_drawingCenterPos) * k;
		setDrawingCenterPos(m_drawingCenterPos.movedBy(diff));

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
		} else if (m_textureRegion.x + m_textureRegion.w < m_rect.x + m_rect.w) {
			m_drawingCenterPos.x = m_rect.x + m_rect.w - m_textureRegion.w * 0.5;
		}
	}

	if (m_textureRegion.h <= m_rect.h) {
		m_drawingCenterPos.y = center.y;
	}
	else {
		if (m_textureRegion.y > m_rect.y) {
			m_drawingCenterPos.y = m_rect.y + m_textureRegion.h * 0.5;
		} else if (m_textureRegion.y + m_textureRegion.h < m_rect.y + m_rect.h) {
			m_drawingCenterPos.y = m_rect.y + m_rect.h - m_textureRegion.h * 0.5;
		}
	}
}

void UIZStackedImageView::setViewingCenterPixel(const Point& centerPixel) {
	// Current scene position of the pixel that will be centered
	const auto pos = Imaging::PixelToScenePos(centerPixel, m_textureRegion, m_scale);
	const auto movement = m_rect.center() - pos;
	setDrawingCenterPos(m_drawingCenterPos.movedBy(movement));
}

void UIZStackedImageView::setDrawingCenterPos(const Vec2& pos) {
	m_drawingCenterPos = pos;
	if (m_textures) {
		m_textureRegion = m_textures[0].scaled(m_scale).regionAt(m_drawingCenterPos);
		restrictImageMovement();
	}
}