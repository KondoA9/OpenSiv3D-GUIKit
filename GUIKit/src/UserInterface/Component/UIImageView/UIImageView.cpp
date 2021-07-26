#include <GUIKit/UIImageView.h>
#include <GUIKit/Imaging.h>

using namespace s3d::gui;

void UIImageView::initialize() {
	UIRect::initialize();

	addEventListener<MouseEvent::RightDragging>([this] {
		const auto movement = Cursor::Pos() - Cursor::PreviousPos();
		setDrawingCenterPos(m_drawingCenterPos.movedBy(movement));
		}, true);

	addEventListener<MouseEvent::Hovering>([this](const MouseEvent::Hovering& e) {
		if (m_textures) {
			m_cursoredPixel = Imaging::ScenePosToPixel(e.pos, m_textureRegion, m_scale);
			m_preCursoredPixel = Imaging::ScenePosToPixel(e.previousPos, m_textureRegion, m_scale);
			m_cursoredPixel.x = Clamp(m_cursoredPixel.x, 0, m_textures[0].width() - 1);
			m_cursoredPixel.y = Clamp(m_cursoredPixel.y, 0, m_textures[0].height() - 1);
		}
		}, true);

	addEventListener<MouseEvent::Wheel>([this](const MouseEvent::Wheel& e) {
		if (manualScalingEnabled) {
			setScale(m_scaleRate + (Sign(e.wheel) < 0 ? 0.05 : -0.05));
		}
		}, true);
}

void UIImageView::appendImage(const Image& image, double alphaRate) {
	m_textures.push_back(DynamicTexture(image, TextureDesc::Unmipped));
	m_alphas.push_back(255 * alphaRate);

	setDrawingCenterPos(rect().center());

	requestToUpdateLayer();

	m_maxPixel = Max(image.width(), image.height());
	m_maxPixel = Clamp(m_maxPixel, m_maxPixel, m_maxPixel);
}

void UIImageView::removeImage(size_t index) {
	m_textures.remove_at(index);
	m_alphas.remove_at(index);
}

void UIImageView::release() {
	releaseImages();
	UIRect::release();
}

void UIImageView::releaseImages() {
	m_textures.release();
	m_alphas.release();
	setDrawingCenterPos(rect().center());
}

void UIImageView::draw() {
	UIRect::draw();

	if (m_textures) {
		m_textureRegion = m_textures[0].scaled(m_scale).regionAt(m_drawingCenterPos);
	}

	restrictImageMovement();

	for (size_t i : step(m_textures.size())) {
		m_textures[i].scaled(m_scale).drawAt(m_drawingCenterPos, Color(255, 255, 255, static_cast<uint32>(m_alphas[i])));
	}
}

void UIImageView::updateLayer(const Rect& scissor) {
	const double preMinScale = m_minScale;
	const double preScale = m_scale;

	UIRect::updateLayer(scissor);

	if (m_textures) {
		m_minScale = calcMinimumScale();
		m_maxScale = calcMaximumScale();

		m_scale = preScale * m_minScale / preMinScale;
		m_scale = Clamp(m_scale, m_minScale, m_maxScale);

		setDrawingCenterPos(m_drawingCenterPos);
	}
}

double UIImageView::calcMinimumScale() {
	double scale = static_cast<double>(rect().w) / static_cast<double>(m_textures[0].width());
	if (const double h = scale * m_textures[0].height(); h > rect().h) {
		scale *= rect().h / h;
	}
	return scale;
}

double UIImageView::calcMaximumScale() {
	return m_minScale * m_maxPixel / m_minPixel;
}

void UIImageView::setScale(double rate) {
	m_scaleRate = Clamp(rate, 0.0, 1.0);

	const double preScale = m_scale;

	m_scale = m_minScale * m_maxPixel / (m_maxPixel - (m_maxPixel - m_minPixel) * m_scaleRate);

	if (m_scale != preScale) {
		const auto diff = (rect().center() - m_drawingCenterPos) * (1.0 - m_scale / preScale);
		setDrawingCenterPos(m_drawingCenterPos.movedBy(diff));
	}
}

void UIImageView::resetScale() {
	m_minScale = calcMinimumScale();
	m_maxScale = calcMaximumScale();
	setScale(0.0);
}

void UIImageView::restrictImageMovement() {
	const auto center = rect().center();

	if (m_textureRegion.w <= rect().w) {
		m_drawingCenterPos.x = center.x;
	}
	else {
		if (m_textureRegion.x > rect().x) {
			m_drawingCenterPos.x = rect().x + m_textureRegion.w * 0.5;
		} else if (m_textureRegion.x + m_textureRegion.w < rect().x + rect().w) {
			m_drawingCenterPos.x = rect().x + rect().w - m_textureRegion.w * 0.5;
		}
	}

	if (m_textureRegion.h <= rect().h) {
		m_drawingCenterPos.y = center.y;
	}
	else {
		if (m_textureRegion.y > rect().y) {
			m_drawingCenterPos.y = rect().y + m_textureRegion.h * 0.5;
		} else if (m_textureRegion.y + m_textureRegion.h < rect().y + rect().h) {
			m_drawingCenterPos.y = rect().y + rect().h - m_textureRegion.h * 0.5;
		}
	}
}

void UIImageView::setViewingCenterPixel(const Point& centerPixel) {
	// Current scene position of the pixel that will be centered
	const auto pos = Imaging::PixelToScenePos(centerPixel, m_textureRegion, m_scale);
	const auto movement = rect().center() - pos;
	setDrawingCenterPos(m_drawingCenterPos.movedBy(movement));
}

void UIImageView::setDrawingCenterPos(const Vec2& pos) {
	m_drawingCenterPos = pos;
	if (m_textures) {
		m_textureRegion = m_textures[0].scaled(m_scale).regionAt(m_drawingCenterPos);
		restrictImageMovement();
	}
}
