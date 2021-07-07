#include <GUIKit/UISlider.h>
#include <GUIKit/PixelUnit.h>

using namespace s3d::gui;

GUICreateInputEvent(Sliding);

UISlider::UISlider(const String& label, UnifiedFontStyle style, TextDirection direction, const ColorTheme& _backgroundColor) :
	UIView(_backgroundColor),
	m_text(UIText(label, style, direction))
{}

void UISlider::initialize() {
	addEventListener<Sliding>([this] {
		const double pre = m_value;
		m_value = Clamp(m_min + (m_max - m_min) * (Cursor::Pos().x - m_layer.left) / m_layer.width, m_min, m_max);

		if (pre != m_value) {
			requestToUpdateLayer();
			if (m_valueChangedHandler) {
				m_valueChangedHandler(m_value);
			}
		}
		}, true);

	addEventListener<MouseEvent::Hovered>([this] {
		handle.backgroundColor.highlight(DynamicColor::DefaultBlue);
		}, true);

	addEventListener<MouseEvent::UnHovered>([this] {
		handle.backgroundColor.lowlight(DynamicColor::Background);
		}, true);

	addEventListener<MouseEvent::Hovering>([] {
		Cursor::RequestStyle(CursorStyle::Hand);
		}, true);

	const double h = 5.0_px;
	railLeft.drawFrame = true;
	railLeft.backgroundColor = DynamicColor::DefaultBlue;
	railLeft.penetrateMouseEvent = true;
	railLeft.setConstraint(LayerDirection::CenterY, handle, LayerDirection::CenterY);
	railLeft.setConstraint(LayerDirection::Height, h);
	railLeft.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	railLeft.setConstraint(LayerDirection::Right, handle, LayerDirection::CenterX);

	railRight.drawFrame = true;
	railRight.backgroundColor = DynamicColor::BackgroundSecondary;
	railRight.penetrateMouseEvent = true;
	railRight.setConstraint(LayerDirection::CenterY, handle, LayerDirection::CenterY);
	railRight.setConstraint(LayerDirection::Height, h);
	railRight.setConstraint(LayerDirection::Left, handle, LayerDirection::CenterX);
	railRight.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

	const double r = 12_px;
	handle.drawFrame = true;
	handle.backgroundColor = DynamicColor::Background;
	handle.penetrateMouseEvent = true;
	handle.setConstraint(LayerDirection::Top, *this, LayerDirection::CenterY);
	handle.setConstraint(LayerDirection::Height, r);
	handle.setConstraint(LayerDirection::CenterX, [this] {
		return m_layer.left + m_layer.width * (m_value - m_min) / (m_max - m_min);
		});
	handle.setConstraint(LayerDirection::Width, r);

	m_text.penetrateMouseEvent = true;
	m_text.setConstraint(LayerDirection::Top, *this, LayerDirection::Top);
	m_text.setConstraint(LayerDirection::Bottom, handle, LayerDirection::Top);
	m_text.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	m_text.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

	appendComponent(railLeft);
	appendComponent(railRight);
	appendComponent(handle);
	appendComponent(m_text);

	UIView::initialize();
}

void UISlider::updateInputEvents() {
	if (mouseCondition().left.down) {
		m_sliding = true;
	}

	if (m_sliding) {
		if (MouseL.up()) {
			m_sliding = false;
		}
		else {
			registerInputEvent(Sliding(this));
		}
	}

	UIView::updateInputEvents();
}
