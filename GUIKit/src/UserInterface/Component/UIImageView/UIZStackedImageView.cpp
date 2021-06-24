#include <GUIKit/UIZStackedImageView.h>
#include <GUIKit/Imaging.h>

using namespace s3d::gui;

UIZStackedImageView::UIZStackedImageView(const ColorTheme& _backgroundColor) :
	UIRect(_backgroundColor)
{
	addEventListener<MouseEvent::RightDragging>([](const auto& e) {
		auto self = static_cast<UIZStackedImageView*>(e.component);
		const auto movement = Cursor::Pos() - Cursor::PreviousPos();
		self->setDrawingCenterPos(self->m_drawingCenterPos.movedBy(movement));
		});

	addEventListener<MouseEvent::Hovering>([](const auto& e) {
		auto self = static_cast<UIZStackedImageView*>(e.component);
		if (self->m_textures) {
			self->m_cursoredPixel = Imaging::ScenePosToPixel(Cursor::Pos(), self->m_textureRegion, self->m_scale);
			self->m_preCursoredPixel = Imaging::ScenePosToPixel(Cursor::PreviousPos(), self->m_textureRegion, self->m_scale);
			self->m_cursoredPixel.x = Clamp(self->m_cursoredPixel.x, 0, self->m_textures[0].width() - 1);
			self->m_cursoredPixel.y = Clamp(self->m_cursoredPixel.y, 0, self->m_textures[0].height() - 1);
		}
		});

	addEventListener<MouseEvent::Wheel>([](const auto& e) {
		auto self = static_cast<UIZStackedImageView*>(e.component);
		if (self->manualScalingEnabled) {
			self->setScale(self->m_scaleRate + (Sign(e.wheel) < 0 ? 0.05 : -0.05));
		}
		});
}

void UIZStackedImageView::appendImage(const Image& image, double alphaRate) {
	m_textures.push_back(DynamicTexture(image, TextureDesc::Unmipped));
	m_alphas.push_back(255 * alphaRate);

	setDrawingCenterPos(m_rect.center());

	requestToUpdateLayer();

	m_maxPixel = Max(image.width(), image.height());
	m_maxPixel = Clamp(m_maxPixel, m_maxPixel, m_maxPixel);
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

		setDrawingCenterPos(m_drawingCenterPos);
	}
}

double UIZStackedImageView::calcMinimumScale() {
	double scale = static_cast<double>(m_rect.w) / static_cast<double>(m_textures[0].width());
	if (const double h = scale * m_textures[0].height(); h > m_rect.h) {
		scale *= m_rect.h / h;
	}
	return scale;
}

double UIZStackedImageView::calcMaximumScale() {
	return m_minScale * m_maxPixel / m_minPixel;
}

void UIZStackedImageView::setScale(double rate) {
	m_scaleRate = Clamp(rate, 0.0, 1.0);

	const double preScale = m_scale;

	m_scale = m_minScale * m_maxPixel / (m_maxPixel - (m_maxPixel - m_minPixel) * m_scaleRate);

	if (m_scale != preScale) {
		const auto diff = (m_rect.center() - m_drawingCenterPos) * (1.0 - m_scale / preScale);
		setDrawingCenterPos(m_drawingCenterPos.movedBy(diff));
	}
}

void UIZStackedImageView::resetScale() {
	m_minScale = calcMinimumScale();
	m_maxScale = calcMaximumScale();
	setScale(0.0);
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